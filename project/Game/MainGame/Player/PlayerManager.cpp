#include "PlayerManager.h"

#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>

void PlayerManager::initialize() {
	players.emplace_back();
	operatePlayer = players.begin();
	operatePlayer->initialize();

	SetToConsole("PlayerManager");
}

void PlayerManager::begin() {
	input();
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
}

void PlayerManager::gather() {
	for (Player& player : players) {
		// 操作プレイヤーと同じ場合は処理しない
		if (std::to_address(operatePlayer) == &player) {
			continue;
		}
		//player.set_state();
	}
}

void PlayerManager::eject() {
	Vector3 forward = { inputStickR.x, 0.0f, inputStickR.y };
	float distance = 1;
	Vector3 separatedPlayerPosition = operatePlayer->world_point() +
		RotateVector(forward, operatePlayer->get_transform().rotate) * distance;

	Player& player = players.emplace_back();
	player.initialize(separatedPlayerPosition);
}

#ifdef _DEBUG
#include <imgui.h>
void PlayerManager::SetAttributeImGui() {
	ImGui::Text("%d", players.size());
	ImGui::Text("%f", aimingTimer.time);
}
#endif // _DEBUG
