#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorRushPlay final : public BaseBossBehavior {
public:
	BossBehaviorRushPlay(float rushLength_, float RushSpeed_);

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	float rushAmount;
	Vector3f direction;

	float RushLength;
	float RushSpeed;

	uint32_t animationDurationCount_ = 0;

	void RushEffect();
};

