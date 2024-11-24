#pragma once

#include "BaseBossBehavior.h"

class BossBehaviorKnockback final : public BaseBossBehavior {
public:
	BossBehaviorKnockback(const Vector3f direction_);
	~BossBehaviorKnockback() = default;

public:
	void move() override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	Vector3f direction;
};
