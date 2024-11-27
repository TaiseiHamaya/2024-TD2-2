#pragma once

#include "BossActionManager.h"

class BossActionFirst final : public BossActionManager {
public:
	BossActionFirst();
	~BossActionFirst() = default;

private:
	std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName);
};

