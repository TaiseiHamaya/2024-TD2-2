#include "BossManager.h"

void BossManager::initialize() {
	boss = std::make_unique<Boss>();
	boss->initialize();
	boss->SetToConsole();
}

void BossManager::update() {
	boss->update();
}

void BossManager::update_matrix() {
	boss->update_matrix();
}
