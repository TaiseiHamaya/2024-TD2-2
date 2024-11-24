#pragma once

#include "BossActionManager.h"

class BossActionOne final : public BossActionManager {
public:
	BossActionOne();
	~BossActionOne() = default;

private:
	std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName);
};

