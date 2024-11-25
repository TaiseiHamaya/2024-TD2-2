#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorTongueStart final : public BaseBossBehavior {
public:
	BossBehaviorTongueStart();

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };
};

