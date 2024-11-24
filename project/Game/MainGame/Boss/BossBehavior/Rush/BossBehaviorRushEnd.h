#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorRushEnd final : public BaseBossBehavior {
public:
	BossBehaviorRushEnd();

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
};
