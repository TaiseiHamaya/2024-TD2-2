#pragma once

#include "BossActionManager.h"

class BossActionSecond final : public BossActionManager {
public:
	BossActionSecond();
	~BossActionSecond() = default;

private:
	std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName);

};
