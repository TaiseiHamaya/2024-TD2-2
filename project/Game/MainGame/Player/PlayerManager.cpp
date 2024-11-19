#include "PlayerManager.h"

#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>

#include "PlayerState/PlayerStateEjection.h"
#include "PlayerState/PlayerStateGather.h"

void PlayerManager::initialize() {
	players.emplace_back();
	operatePlayer = players.begin();
	operatePlayer->initialize();

	SetToConsole("PlayerManager");
}

void PlayerManager::begin() {
	isEject = false;
	isAiming = false;
	gatherBitset <<= 1;
	if (operatePlayer->empty_state()) {
		input();
	}
	if (isAiming) {
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
	if (isEject) {
		eject();
	}
	else if(gatherBitset == 0b01 && !isAiming) {
		inputStickL = {0.0f, 0.0f};
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
}

void PlayerManager::update_matrix() {
	for (Player& player : players) {
		player.update_matrix();
	}
}

void PlayerManager::input() {
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


	auto ejectButton = XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_LEFT_SHOULDER;
	isAiming = gamepad->IsPressButton(ejectButton);
	isEject = gamepad->IsReleaseButton(ejectButton);

	gatherBitset.set(0, gamepad->IsPressButton(XINPUT_GAMEPAD_A));
}

void PlayerManager::gather() {
	const QuaternionTransformBuffer* targetAddress = &operatePlayer->get_transform();
	for (Player& player : players) {
		// 操作プレイヤーと同じ場合は処理しない
		if (std::to_address(operatePlayer) == &player) {
			continue;
		}
		player.push_state(
			std::make_unique<PlayerState::Gather>(
				&player.get_transform(), targetAddress
			)
		);
	}
}

void PlayerManager::ungather() {
	for (Player& player : players) {
		// 操作プレイヤーと同じ場合は処理しない
		if (std::to_address(operatePlayer) == &player) {
			continue;
		}
		player.ungather();
	}
}

void PlayerManager::eject() {
	Vector3 forward = { inputStickR.x, 0.0f, inputStickR.y };
	float distance = 1;
	// 開始位置
	Vector3 separatedPlayerPosition = operatePlayer->world_point() +
		RotateVector(forward, operatePlayer->get_transform().transform.rotate) * distance;

	if (Length(forward) >= 0.1f) {
		// 追加
		Player& player = players.emplace_back();
		player.initialize(separatedPlayerPosition);
		player.push_state(
			std::make_unique<PlayerState::Ejection>(Normalize(forward))
		);
	}
}

#ifdef _DEBUG
#include <imgui.h>
void PlayerManager::SetAttributeImGui() {
	ImGui::Text("%d", players.size());
	ImGui::Text("%f", aimingTimer.time);
}
#endif // _DEBUG
