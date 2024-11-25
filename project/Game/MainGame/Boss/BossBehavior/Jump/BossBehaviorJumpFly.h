#pragma once

#include "../BaseBossBehavior.h"

class PlayerManager;

class BossBehaviorJumpFly final : public BaseBossBehavior {
public:
	BossBehaviorJumpFly(float FlyTime_, float ApproachTime_, const PlayerManager* playerManager_);

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	float FlyTime;
	float ApproachTime;
	const PlayerManager* playerManager;
};
