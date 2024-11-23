#pragma once

#include "BaseBossBehavior.h"

class PlayerManager;

class BossBehaviorStay final : public BaseBossBehavior {
public:
	BossBehaviorStay(float Time, const PlayerManager* playerManager_);

public:
	void move() override;

private:
	float stayTime;
	const PlayerManager* playerManager;
};
