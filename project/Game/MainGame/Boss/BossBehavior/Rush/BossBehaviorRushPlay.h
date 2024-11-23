#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorRushPlay final : public BaseBossBehavior {
public:
	BossBehaviorRushPlay(float rushLength_, float RushSpeed_);

public:
	void move() override;

public:

private:
	float rushAmount;
	Vector3f direction;

	float RushLength;
	float RushSpeed;
};

