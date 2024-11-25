#pragma once

#include "../BaseBossBehavior.h"

class BossBehaviorTonguePlay final : public BaseBossBehavior {
public:
	BossBehaviorTonguePlay(float length, float Time_);

public:
	void move() override;
	void update_collider(const Vector3f& position) override;

public:
	constexpr bool allow_interruption() override { return false; };

private:
	void apply_collider(const Vector3f& direction);

private:
	Quaternion startRotation;
	float Time;
	float colliderLength;
};

