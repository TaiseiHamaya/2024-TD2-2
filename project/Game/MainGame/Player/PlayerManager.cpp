#include "PlayerManager.h"

#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>

void PlayerManager::initialize() {
	players.emplace_back();
	operatePlayer = players.begin();
	operatePlayer->initialize();
}

void PlayerManager::begin() {
	input();
	for (Player& player : players) {
		player.begin();
	}
}

void PlayerManager::update() {
	// 操作プレイヤーの更新
	operatePlayer->operate_update(inputStick);
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
	// 入力情報の取得
	inputStick =
		SxavengerEngine::GetInput()->
		GetGamepadInput(0)->
		GetLStickNormalize();
	// デッドゾーン以下の場合は0ベクトルにする
	if (Length(inputStick) <= 0.1f) {
		inputStick = { 0.0f,0.0f };
	}
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
