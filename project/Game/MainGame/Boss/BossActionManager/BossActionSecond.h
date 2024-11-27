#pragma once

#include "BossActionManager.h"

class BossActionSecond final : public BossActionManager {
public:
	BossActionSecond();
	~BossActionSecond() = default;

private:
	std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName);

#ifdef _DEBUG
public:
	void SetAttributeImGui() override;
#endif // _DEBUG

private:
	float StayTime;

	float LookAtStartTime;
	float RushStartTime;
	float RushSpeed;
	float RushLength;

	float JumpHeight;
	float FlyingTime;
	float ApproachTime;
	float ApproachSpeed;
	float StompTime;
};
