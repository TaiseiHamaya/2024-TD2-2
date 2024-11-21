#include "BossManager.h"

void BossManager::initialize() {
	boss = std::make_unique<Boss>(30);
	boss->SetToConsole();
}

void BossManager::update() {
	if (!boss) {
		return;
	}
	boss->update();
	if (boss->is_dead()) {
		boss->set_invincible(true);
		isTransition = true;
	}
	if (boss->is_destroy()) {
		//initialize_boss();
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

void BossManager::create_model() {
	if (!boss) {
		return;
	}


}
