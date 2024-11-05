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

private:
	std::unique_ptr<Boss> boss;
};
