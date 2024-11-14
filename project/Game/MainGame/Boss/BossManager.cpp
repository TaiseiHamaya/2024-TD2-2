#include "BossManager.h"

void BossManager::initialize() {
	boss = std::make_unique<Boss>();
	boss->initialize();
	boss->SetToConsole();
}

void BossManager::update() {
	if (boss) {
		boss->update();
	}
}

void BossManager::update_matrix() {
	if (boss) {
		boss->update_matrix();
	}
}
