#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorRushStart final : public BaseBossBehavior {
public:
	BossBehaviorRushStart(const Quaternion& rushDirection, float LookStartTime_, float RushStartTime_);

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	Quaternion beginRotation;
	Quaternion endRotation;

	float LookStartTime;
	float RushStartTime;
};
