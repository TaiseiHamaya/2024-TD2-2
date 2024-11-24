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
	void begin();
	void update();
	void update_matrix();

	void attack_hit_callback();
	void damaged_hit_callback(int32_t damage, const Vector3f& playerTranslate);

public:
	bool is_Invincible() const;
	Collider* get_attack_collider() const;
	Collider* get_hit_collider() const;

	Boss* GetBoss() const { return boss.get(); }

private:

private:
	std::unique_ptr<Boss> boss;
	std::unique_ptr<BossActionManager> bossActionManager;
	int32_t wave;
	bool isTransition{ false };
	int32_t MaxWave;
};
