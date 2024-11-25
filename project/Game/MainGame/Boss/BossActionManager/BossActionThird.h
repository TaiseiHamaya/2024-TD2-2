#pragma once

#include "BossActionManager.h"

class BossActionThird : public BossActionManager {
public:
	BossActionThird();
	~BossActionThird() = default;

private:
	std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName);

};

