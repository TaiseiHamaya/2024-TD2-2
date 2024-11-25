#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorJumpJump final : public BaseBossBehavior {
public:
	BossBehaviorJumpJump(float JumpHeight);

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	float JumpTime;
	float JumpHeight;
};
