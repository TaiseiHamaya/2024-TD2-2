#include "BossManager.h"

#include "BossActionManager/BossActionOne.h"

#include "BossBehavior/BossBehaviorStay.h"

void BossManager::initialize(const PlayerManager* player) {
	boss = std::make_unique<Boss>(30);
	boss->SetToConsole();
	bossActionManager = std::make_unique<BossActionOne>();
	BaseBossBehavior::boss = boss.get();
	BossActionManager::playerManager = player;

	boss->set_behavior(std::make_unique<BossBehaviorStay>(5.0f, player));
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

void BossManager::boss_damage_callback(int32_t damage) {
	if (boss) {
		boss->take_damage(damage);
	}
}
