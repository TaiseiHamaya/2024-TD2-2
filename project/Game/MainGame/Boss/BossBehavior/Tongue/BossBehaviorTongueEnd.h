#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorTongueEnd : public BaseBossBehavior {
public:
	BossBehaviorTongueEnd();

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

};
