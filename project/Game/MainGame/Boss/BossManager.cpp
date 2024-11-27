#include "BossManager.h"

#include "BossActionManager/BossActionFirst.h"
#include "BossActionManager/BossActionSecond.h"
#include "BossActionManager/BossActionThird.h"

#include "BossBehavior/BossBehaviorKnockback.h"
#include "BossBehavior/BossBehaviorStay.h"
#include "BossBehavior/BossBehaviorAnimation.h"

#include "../Player/PlayerAutomationPop.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>


#include <Engine/System/Performance.h>
#include <Lib/Easing.h>

void BossManager::initialize(const PlayerManager* player, PlayerAutomationPop* playerAutomationPop_) {
	boss = std::make_unique<Boss>();
	boss->SetToConsole();

	BaseBossBehavior::boss = boss.get();
	BossActionManager::playerManager = player;
	playerAutomationPop = playerAutomationPop_;

	BaseBehavior::renderingFlag_ = kBehaviorRender_LateAdaptive;

	phase = 0;
	isPhaseTransition = false;

	initialize_action();

	SetName("BossManager");
	SetToConsole();

	hpFrameTexture_     = Sxavenger::LoadTexture("resourcesData/gameScene/Model/HP_frame.png");
	hpFrameBackTexture_ = Sxavenger::LoadTexture("resourcesData/gameScene/Model/HP_bar_back.png");

	hpTexture_[0] = Sxavenger::LoadTexture("resourcesData/gameScene/Model/HP_bar1.png");
	hpTexture_[1] = Sxavenger::LoadTexture("resourcesData/gameScene/Model/HP_bar2.png");
	hpTexture_[2] = Sxavenger::LoadTexture("resourcesData/gameScene/Model/HP_bar3.png");
}

void BossManager::begin() {
	if (boss) {
		boss->begin();
	}
}

void BossManager::update() {
	if (!boss || isEndAll) {
		return;
	}
	boss->update();
	if (bossActionManager) {
		bossActionManager->update();
	}

	// トランジション開始
	if (boss->is_dead() && !isPhaseTransition) {
		next_phase();
		playerAutomationPop->reset();
	}
	// トランジション終了
	else if (isPhaseTransition && boss->is_end_behavior()) {
		isPhaseTransition = false;
		if (!bossActionManager) {
			isEndAll = true;
		}
	}
	// 通常処理
	if (boss->is_end_behavior()) {
		if (bossActionManager) {
			boss->set_behavior(bossActionManager->next());
		}
	}

}

void BossManager::update_matrix() {
	if (boss) {
		boss->update_matrix();
	}
}

void BossManager::attack_hit_callback() {
	if (boss) {
		// コリジョンコールバック
		boss->get_behavior()->any_hit_collision();
	}
}

void BossManager::damaged_hit_callback(float damage, const Vector3f& playerTranslate) {
	if (boss) {
		boss->hit_callback();
		boss->take_damage(damage);

		// 現在の行動を中断可能なら強制的にノックバック状態にする
		if (boss->get_behavior()->allow_interruption()) {
			Vector3f direction = Normalize(boss->get_transform().GetWorldPosition() - playerTranslate);
			boss->set_behavior(std::make_unique<BossBehaviorKnockback>(direction));
			bossActionManager->cancel_now_action();
		}
	}
}

void BossManager::DrawAdaptive(_MAYBE_UNUSED const Camera3D* camera) {

	auto drawer = SxavengerGame::GetSpriteCommon();

	{

		Vector2f position = { kWindowSize.x - hpFrameBackTexture_->GetSize().x / 2.0f, kWindowSize.y / 2.0f };

		drawer->DrawSprite(
			position, hpFrameBackTexture_->GetSize(), { 0.5f, 0.5f }, hpFrameBackTexture_->GetGPUHandleSRV(), { 0.2f, 0.2f, 0.2f, 1.0f }
		);
	}

	DrawHP(phase);

	{ //!< health point frame
		Vector2f position = { kWindowSize.x - hpFrameTexture_->GetSize().x / 2.0f, kWindowSize.y / 2.0f };

		drawer->DrawSprite(
			position, hpFrameTexture_->GetSize(), { 0.5f, 0.5f }, hpFrameTexture_->GetGPUHandleSRV()
		);
	}
}

bool BossManager::is_end_all() const {
	return isEndAll;
}

bool BossManager::is_Invincible() const {
	if (isPhaseTransition || boss->is_invincible()) {
		return true;
	}
	return false;
}

bool BossManager::is_transition() const {
	return isPhaseTransition && bossActionManager;
}

Collider* BossManager::get_attack_collider() const {
	return boss->get_attack_collider();
}

Collider* BossManager::get_hit_collider() const {
	return boss->get_hit_collider();
}

void BossManager::next_phase() {
	++phase;
	initialize_action();
	boss->set_invincible(true);
	isPhaseTransition = true;
}

void BossManager::initialize_action() {
	switch (phase) {
	case 0: // P1
		bossActionManager = std::make_unique<BossActionFirst>();
		boss->set_behavior(std::make_unique<BossBehaviorAnimation>("enemy_gameStart.gltf"));
		boss->set_phase_uv(phase);
		break;
	case 1: // P2
		bossActionManager = std::make_unique<BossActionSecond>();
		boss->set_behavior(std::make_unique<BossBehaviorAnimation>("enemy_gameStart.gltf"));
		boss->set_phase_uv(phase);
		break;
	case 2: // P3
		bossActionManager = std::make_unique<BossActionThird>();
		boss->set_behavior(std::make_unique<BossBehaviorAnimation>("enemy_gameStart.gltf"));
		boss->set_phase_uv(phase);
		break;
	case MaxWave:
		bossActionManager.reset();
		boss->set_behavior(std::make_unique<BossBehaviorAnimation>("enemy_death.gltf"));
		//boss->set_behavior(std::make_unique<>());
		return;
		break;
	}
	boss->reset_hitpoint(bossActionManager->max_hitpoint());
}

void BossManager::DrawHP(int32_t wave) {
	auto drawer = SxavengerGame::GetSpriteCommon();
	if (wave == MaxWave) {
		return;
	}

	// back
	if (wave < MaxWave - 1) {
		int32_t nextWave = wave + 1;

		Vector2f position = { kWindowSize.x - hpTexture_[nextWave]->GetSize().x / 2.0f, kWindowSize.y / 2.0f};

		drawer->DrawSprite(
			position, hpTexture_[nextWave]->GetSize(), {0.5f, 0.5f}, hpTexture_[nextWave]->GetGPUHandleSRV()
		);
	}

	// front
	Vector2f frameSize = hpTexture_[wave]->GetSize();

	float hpT = (boss->GetHitPoint()) / static_cast<float>(bossActionManager->max_hitpoint());
	hpT = std::clamp(hpT + 0.1f, 0.0f, 1.0f);

	Vector2f position = { kWindowSize.x - frameSize.x, kWindowSize.y / 2.0f + frameSize.y / 2.0f };
	Vector2f size = { frameSize.x, -std::lerp(0.0f, frameSize.y, hpT) };

	drawer->DrawSpriteClip(
		position, size, { 0.0f, 0.0f }, { 1.0f, hpT }, hpTexture_[wave]->GetGPUHandleSRV()
	);
	
}

#ifdef _DEBUG
void BossManager::SetAttributeImGui() {
	ImGui::Text("Phase");
	bool isChanged = false;
	if (ImGui::RadioButton("First", phase == 0)) {
		phase = 0;
		isChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Second", phase == 1)) {
		phase = 1;
		isChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Third", phase == 2)) {
		phase = 2;
		isChanged = true;
	}
	if (isChanged) {
		initialize_action();
	}
}


#endif // DEBUG
