#include "PlayerManager.h"

#include <Engine/Console/SystemConsole.h>
#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>
#include <Engine/Beta/ImGuiJsonExporter.h>

#include "PlayerState/PlayerStateEjection.h"
#include "PlayerState/PlayerStateGather.h"

PlayerManager::~PlayerManager() = default;

void PlayerManager::initialize() {
	SetToConsole("PlayerManager");

	exporter_.TryLoadFromJson();
	exporter_.GetFromStash("MaxSize", &Player::maxSize, 1);
	exporter_.GetFromStash("MinSize", &Player::minSize, 1);
	exporter_.GetFromStash("ModelSize", &Player::ModelSize, 1);
	exporter_.GetFromStash("DefaultSize", &Player::DefaultSize, 1);
	exporter_.GetFromStash("SizeParSec", &Player::SizeParSec, 1);

	exporter_.GetFromStash("EjectMaxDistance", &EjectMaxDistance, 1);
	exporter_.GetFromStash("EjectLengthParSecond", &EjectLengthParSecond, 1);

	players.emplace_back();
	operatePlayer = std::to_address(players.begin());
	operatePlayer->initialize(kOrigin3, Player::DefaultSize);
	canEject = true;

	exporter_.TryLoadFromJson();

	cursol_ = std::make_unique<Cursol>();
	cursol_->Init();
	SetChild(cursol_.get());

	PlayerState::Gather::playerManager = this;

}

void PlayerManager::begin() {
	ejectBitset <<= 1;
	gatherBitset <<= 1;
	input();
	if (ejectBitset.any()) {
		aimingTimer.AddDeltaTime();
	}
	else {
		// リセット
		aimingTimer.time = 0;
	}
	for (Player& player : players) {
		player.begin();
	}
}

void PlayerManager::update() {
	if (ejectBitset == 0b10) {
		eject();
	}
	if (gatherBitset == 0b01) {
		inputStickL = { 0.0f, 0.0f };
		gather();
	}
	else if (gatherBitset == 0b10) {
		ungather();
	}

	// 操作プレイヤーの更新
	operatePlayer->operate_update(inputStickL);

	// 通常更新
	for (Player& player : players) {
		player.update();
	}

	cursol_->Update(operatePlayer->world_point());
}

void PlayerManager::update_matrix() {
	for (Player& player : players) {
		player.update_matrix();
	}

	cursol_->UpdateMatrix();
}

void PlayerManager::marge_collision() {
	if (players.empty()) {
		return;
	}
	for (auto lhs = players.begin(); lhs != std::prev(players.end()); ++lhs) {
		for (auto rhs = std::next(lhs); rhs != players.end(); ++rhs) {
			if (
				lhs->get_hit_collider()->GetStates(rhs->get_hit_collider().get()).test(0) &&
				rhs->get_hit_collider()->GetStates(lhs->get_hit_collider().get()).test(0)
				) {
				Vector3f margeTranslate =
					lhs->get_transform().GetWorldPosition() +
					rhs->get_transform().GetWorldPosition();
				margeTranslate /= 2.0f;
				float mergedSize =
					lhs->get_size() + rhs->get_size();

				rhs = players.erase(rhs);
				lhs = players.erase(lhs);

				auto& newPlayer = players.emplace_back();
				newPlayer.initialize(margeTranslate, mergedSize);
				if (rhs == players.end()) {
					break;
				}
			}
		}
		if (lhs == players.end()) {
			break;
		}
	}

	// 次の操作キャラクターの設定
	search_operate_player();
}

void PlayerManager::input() {
	if (!operatePlayer->empty_state()) {
		return;
	}
	const GamepadInput* gamepad = SxavengerEngine::GetInput()->GetGamepadInput(0);
	// 入力情報の取得
	// スティック入力
	inputStickL = gamepad->GetLStickNormalize();
	inputStickR = gamepad->GetRStickNormalize();
	// デッドゾーン以下の場合は0ベクトルにする
	if (Length(inputStickL) <= 0.1f) {
		inputStickL = { 0.0f,0.0f };
	}
	if (Length(inputStickR) <= 0.1f) {
		inputStickR = { 0.0f,0.0f };
	}

	gatherBitset.set(0, gamepad->IsPressButton(XINPUT_GAMEPAD_A));

	float playerSize = operatePlayer->get_size();
	if (!gatherBitset.test(0) && playerSize >= Player::minSize * 2) {
		auto ejectButton = XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_LEFT_SHOULDER;
		ejectBitset.set(0, gamepad->IsPressButton(ejectButton));
	}

	if (gatherBitset.test(0)) {
		inputStickL *= 0.5f;
	}
}

void PlayerManager::gather() {
	for (Player& player : players) {
		// 操作プレイヤーと同じ場合は処理しない
		if (operatePlayer == &player) {
			continue;
		}
		player.push_state(
			std::make_unique<PlayerState::Gather>(
				&player.get_transform()
			)
		);
	}
}

void PlayerManager::ungather() {
	for (Player& player : players) {
		// 操作プレイヤーと同じ場合は処理しない
		if (operatePlayer == &player) {
			continue;
		}
		player.ungather();
	}
}

void PlayerManager::eject() {
	Vector3 forward = { inputStickR.x, 0.0f, inputStickR.y };

	if (Length(forward) < 0.1f) {
		return;
	}

	float playerSize = operatePlayer->get_size();
	float magnification = std::min(aimingTimer.time, (playerSize - Player::minSize) / Player::SizeParSec);
	// 大きさ算出
	float ejectSize = magnification * Player::SizeParSec;
	// 距離算出
	float ejectDistance = EjectMaxDistance - magnification * EjectLengthParSecond;
	float distance = playerSize / 2;
	// 開始位置
	Vector3 separatedPlayerPosition = operatePlayer->world_point() +
		RotateVector(forward, operatePlayer->get_transform().transform.rotate) * distance;
	// 追加
	Player& newPlayer = players.emplace_back();
	newPlayer.initialize(separatedPlayerPosition, ejectSize);
	newPlayer.push_state(
		std::make_unique<PlayerState::Ejection>(Normalize(forward), ejectDistance)
	);
	// 集合命令が入っている場合は、新しいやつにも適用する
	if (gatherBitset.test(0)) {
		newPlayer.push_state(
			std::make_unique<PlayerState::Gather>(
				&newPlayer.get_transform()
			)
		);
	}
	// 分裂元のサイズ変更
	operatePlayer->set_sizing(playerSize - ejectSize);

	// 次の操作キャラクターの設定
	search_operate_player();
}

void PlayerManager::search_operate_player() {
	float nextOperateSize = -1;
	for (Player& player : players) {
		float size = player.get_size();
		if (size > nextOperateSize) {
			nextOperateSize = size;
			operatePlayer = &player;
		}
	}
}

#ifdef _DEBUG
#include <imgui.h>
void PlayerManager::SetAttributeImGui() {
	ImGui::Text("%d", players.size());
	ImGui::Text("%f", aimingTimer.time);
	ImGui::Separator();
	exporter_.DragFloat("MaxSize", &Player::maxSize, 0.05f);
	exporter_.DragFloat("MinSize", &Player::minSize, 0.05f);
	exporter_.DragFloat("ModelSize", &Player::ModelSize, 0.05f);
	exporter_.DragFloat("DefaultSize", &Player::DefaultSize, 0.05f);
	exporter_.DragFloat("SizeParSec", &Player::SizeParSec, 0.05f);

	exporter_.DragFloat("EjectMaxDistance", &EjectMaxDistance, 0.05f);
	exporter_.DragFloat("EjectLengthParSecond", &EjectLengthParSecond, 0.05f);

	if (ImGui::Button("output parameter")) {
		exporter_.OutputToJson();
	}
}
#endif // _DEBUG
