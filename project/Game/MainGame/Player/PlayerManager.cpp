#include "PlayerManager.h"

#include <Engine/Console/SystemConsole.h>
#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>
#include <Engine/Beta/ImGuiJsonExporter.h>

#include "PlayerState/PlayerStateEjection.h"
#include "PlayerState/PlayerStateGather.h"
#include "PlayerState/PlayerStateSpawn.h"

PlayerManager::~PlayerManager() = default;

void PlayerManager::initialize() {
	SetToConsole("PlayerManager");

	exporter_.TryLoadFromJson();
	exporter_.GetFromStash("MinSize", &Player::MinSize, 1);
	exporter_.GetFromStash("ModelSize", &Player::ModelSize, 1);
	exporter_.GetFromStash("DefaultSize", &Player::DefaultSize, 1);
	exporter_.GetFromStash("MoveSpeed", &Player::MoveSpeed, 1);

	exporter_.GetFromStash("EjectPressTime", &EjectPressTime, 1);
	exporter_.GetFromStash("EjectMaxDistance", &EjectMaxDistance, 1);
	exporter_.GetFromStash("EjectLengthParSecond", &EjectMaxDistance, 1);

	exporter_.GetFromStash("MinDamage", &Player::MinDamage, 1);
	exporter_.GetFromStash("MaxDamage", &Player::MaxDamage, 1);

	players.emplace_back();
	operatePlayer = std::to_address(players.begin());
	operatePlayer->initialize(Vector3f{ 0.0f,0.0f,-30.0f }, Player::DefaultSize);
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

	if (operatePlayer->IsLanding()) {
		Sxavenger::PlayAudioOneShot("player_jump.wav", 0.2f);
	}
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

void PlayerManager::spawn(std::list<Player>&& spawnPlayers) {
	// stateの設定
	for (auto& spawnPlayer : spawnPlayers) {
		spawnPlayer.push_state(
			std::make_unique<PlayerState::Spawn>()
		);
	}

	// 移動
	players.splice(players.end(), spawnPlayers);
}

bool PlayerManager::CanShot() const {
	float playerSize = operatePlayer->get_size();
	return gatherBitset.none() && playerSize >= Player::MinSize * 2;
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

	if (ejectBitset.none()) {
		gatherBitset.set(0, gamepad->IsPressButton(XINPUT_GAMEPAD_A));
	}

	if (CanShot()) {
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

template<typename T>
float LerpInverse(const T& min, const T& max, const T& value) {
	if (min == max) {
		return min;
	}

	return (value - min) / (max - min);
}

void PlayerManager::eject() {
	Vector3 forward = { inputStickR.x, 0.0f, inputStickR.y };

	if (Length(forward) < 0.1f) {
		return;
	}

	float playerSize = operatePlayer->get_size();
	const float DefaultMaxSize = Player::DefaultSize - Player::MinSize;
	const float MaxEjectSize = playerSize - Player::MinSize;
	float maxParametric = LerpInverse(Player::MinSize, DefaultMaxSize, MaxEjectSize);
	float parametric = std::min(maxParametric, aimingTimer.time / EjectPressTime);

	// 大きさ算出
	float ejectSize = std::lerp(Player::MinSize, DefaultMaxSize, parametric);
	// 距離算出
	float ejectDistance = std::lerp(EjectMaxDistance, EjectMinDistance, std::min(parametric, 1.0f));
	float EjectStartDistance = playerSize / 2;
	// 開始位置
	Vector3 separatedPlayerPosition = operatePlayer->world_point() +
		forward * EjectStartDistance;
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

	Sxavenger::PlayAudioOneShot("player_injection.wav", 0.2f);
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
	exporter_.DragFloat("MinSize", &Player::MinSize, 0.1f);
	exporter_.DragFloat("ModelSize", &Player::ModelSize, 0.1f);
	exporter_.DragFloat("DefaultSize", &Player::DefaultSize, 0.1f);
	exporter_.DragFloat("MoveSpeed", &Player::MoveSpeed, 0.1f);

	exporter_.DragFloat("EjectPressTime", &EjectPressTime, 0.01f);
	exporter_.DragFloat("EjectMaxDistance", &EjectMaxDistance, 0.1f);
	exporter_.DragFloat("EjectMinDistance", &EjectMinDistance, 0.1f);

	exporter_.DragFloat("MinDamage", &Player::MinDamage, 0.1f);
	exporter_.DragFloat("MaxDamage", &Player::MaxDamage, 0.1f);

	if (ImGui::Button("output parameter")) {
		exporter_.OutputToJson();
	}
}
#endif // _DEBUG
