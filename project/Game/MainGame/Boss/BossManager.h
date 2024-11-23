#pragma once

#include "Boss.h"
#include "BossActionManager/BossActionManager.h"

class PlayerManager;

class BossManager {
public:
	BossManager() = default;
	~BossManager() = default;

	BossManager(const BossManager&) = delete;
	BossManager& operator=(const BossManager&) = delete;

public:
	void initialize(const PlayerManager* player);
	void update();
	void update_matrix();

	void boss_damage_callback(int32_t damage);

private:

private:
	std::unique_ptr<Boss> boss;
	std::unique_ptr<BossActionManager> bossActionManager;
	int32_t wave;
	bool isTransition{ false };
	int32_t MaxWave;
};
