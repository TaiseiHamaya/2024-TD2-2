#include "BossManager.h"

#include "BossActionManager/BossActionFirst.h"
#include "BossActionManager/BossActionSecond.h"
#include "BossActionManager/BossActionThird.h"

#include "BossBehavior/BossBehaviorKnockback.h"
#include "BossBehavior/BossBehaviorStay.h"

void BossManager::initialize(const PlayerManager* player) {
	boss = std::make_unique<Boss>();
	boss->SetToConsole();

	BaseBossBehavior::boss = boss.get();
	BossActionManager::playerManager = player;

	phase = 0;
	isPhaseTransition = false;

	initialize_action();
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
	bossActionManager->update();

	// トランジション開始
	if (boss->is_dead() && !isPhaseTransition) {
		next_phase();
	}
	// トランジション終了
	else if (isPhaseTransition && boss->is_end_behavior()) {
		isPhaseTransition = false;
		if (!bossActionManager) {
			boss.reset();
			isEndAll = true;
		}
	}
	// 通常処理
	if (boss->is_end_behavior()) {
		boss->set_behavior(bossActionManager->next());
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

void BossManager::damaged_hit_callback(int32_t damage, const Vector3f& playerTranslate) {
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

bool BossManager::is_end_all() const {
	return isEndAll;
}

bool BossManager::is_Invincible() const {
	if (isPhaseTransition || boss->is_invincible()) {
		return true;
	}
	return false;
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
		boss->set_behavior(std::make_unique<BossBehaviorStay>(5.0f, nullptr));
		break;
	case 1: // P2
		bossActionManager = std::make_unique<BossActionSecond>();
		boss->set_behavior(std::make_unique<BossBehaviorStay>(5.0f, nullptr));
		break;
	case 2: // P3
		bossActionManager = std::make_unique<BossActionThird>();
		boss->set_behavior(std::make_unique<BossBehaviorStay>(5.0f, nullptr));
		break;
	case MaxWave:
		bossActionManager.reset();
		//boss->set_behavior(std::make_unique<>());
		return;
		break;
	}
	boss->reset_hitpoint(bossActionManager->max_hitpoint());
}
