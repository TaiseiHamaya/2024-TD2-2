#pragma once

#include "BaseBossBehavior.h"

#include <string>

class BossBehaviorAnimation : public BaseBossBehavior {
public:
	BossBehaviorAnimation(const std::string& animationName);
	~BossBehaviorAnimation() = default;

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };
};
