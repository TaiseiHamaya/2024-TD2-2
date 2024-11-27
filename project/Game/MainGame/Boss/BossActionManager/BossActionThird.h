#pragma once

#include "BossActionManager.h"

class BossActionThird : public BossActionManager {
public:
	BossActionThird();
	~BossActionThird() = default;

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
	float ColliderLength;
	float TurnTime;
};

