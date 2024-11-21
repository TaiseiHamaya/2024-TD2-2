#pragma once

#include "Boss.h"

class BossManager {
public:
	BossManager() = default;
	~BossManager() = default;

	BossManager(const BossManager&) = delete;
	BossManager& operator=(const BossManager&) = delete;

public:
	void initialize();
	void update();
	void update_matrix();

	void boss_damage_callback(int32_t damage);

private:
	void create_model();

private:
	std::unique_ptr<Boss> boss;
	int32_t wave;
	bool isTransition{ false };
	int32_t MaxWave;
};
