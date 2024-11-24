#include "MainGameScene.h"

#include <Engine/Game/Camera/Camera3D.h>
#include <Engine/Console/SystemConsole.h>
#include <Engine/Game/SxavengerGame.h>

#include <Lib/Geometry/MathLib.h>

#include "Game/MainGame/Player/PlayerState/PlayerStateKnockback.h"

void MainGameScene::Init() {

	playerManager = std::make_unique<PlayerManager>();
	playerManager->initialize();

	bossManager = std::make_unique<BossManager>();
	bossManager->initialize(playerManager.get());

	field_ = std::make_unique<Field>();
	field_->Init();
	field_->SetToConsole();

	//SystemConsole::GetInstance()->GetGameCamera()->SetTransform(kUnit3, Vector3{ 45.0f * kDegToRad ,0.0f,0.0f }, Vector3{ 0,10,-10 });

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Init();
	gameCamera_->SetToConsole();
}

void MainGameScene::Term() {
}

void MainGameScene::Update() {
	// 開始処理
	playerManager->begin();
	bossManager->begin();
	// 更新処理
	playerManager->update();
	bossManager->update();
	// 行列更新
	playerManager->update_matrix();
	bossManager->update_matrix();

	SxavengerGame::UpdateColliders();
	field_->Update();
	// 衝突処理
	collision();
}

void MainGameScene::Draw() {
}

void MainGameScene::collision() {
	// プレイヤー同士の接触
	playerManager->marge_collision();
	// 敵攻撃とプレイヤーの接触
	collision_boss_attack();
	// プレイヤー攻撃と敵の接触
	collision_player_attack();
}

void MainGameScene::collision_boss_attack() {
	Collider* bossCollider = bossManager->get_attack_collider();
	std::list<Player>& players = playerManager->get_players();
	for (auto& player : players) {
		// 攻撃コライダがアクティブ時はダメージを受けない
		if (player.get_attack_collider()) {
			continue;
		}
		// 衝突判定
		else if (player.get_hit_collider()->GetStates(bossCollider).test(0)) {
			// プレイヤー側の処理
			// 向き取得
			Vector3f direction = player.get_transform().GetWorldPosition() - bossCollider->GetColliderPosition();
			// 正規化
			direction = Length(direction) >= 0.05f ? Normalize(direction) : Vector3f{ 0, 0, 1 };
			// ステータス設定
			player.push_state(
				std::make_unique<PlayerState::Knockback>(direction)
			);
			player.take_damage();

			// 敵側の処理
			bossManager->attack_hit_callback();
			return;
		}
	}
}

void MainGameScene::collision_player_attack() {
	// ボスのコリジョンが存在しない場合は戻る
	if (bossManager->is_Invincible()) {
		return;
	}
	Collider* bossCollider = bossManager->get_hit_collider();
	std::list<Player>& players = playerManager->get_players();
	for (auto& player : players) {
		if (bossCollider->GetStates(player.get_attack_collider()).test(0)) {
			bossManager->damaged_hit_callback(1, player.world_point());
		}
	}
}
