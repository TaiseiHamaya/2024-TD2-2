#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorJumpStomp final : public BaseBossBehavior {
public:
	BossBehaviorJumpStomp(float StompTime_);

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	float StompTime;
	float StartHeight;

	void CreateStompEffect();
};

