#include "BossManager.h"

#include "BossActionManager/BossActionOne.h"
#include "BossActionManager/BossActionSecond.h"
#include "BossActionManager/BossActionThird.h"

#include "BossBehavior/BossBehaviorStay.h"
#include "BossBehavior/BossBehaviorKnockback.h"

void BossManager::initialize(const PlayerManager* player) {
	boss = std::make_unique<Boss>(30);
	boss->SetToConsole();
	bossActionManager = std::make_unique<BossActionSecond>();
	BaseBossBehavior::boss = boss.get();
	BossActionManager::playerManager = player;

	boss->set_behavior(std::make_unique<BossBehaviorStay>(5.0f, player));
}

void BossManager::begin() {
	if (boss) {
		boss->begin();
	}
}

void BossManager::update() {
	if (!boss) {
		return;
	}
	boss->update();
	bossActionManager->update();

	if (boss->is_dead()) {
		boss->set_invincible(true);
		isTransition = true;
	}
	if (boss->is_destroy()) {
		//initialize_boss();
	}
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

bool BossManager::is_Invincible() const {
	if (isTransition || boss->is_invincible()) {
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
