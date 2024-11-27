#pragma once

#include "BossActionManager.h"

class BossActionFirst final : public BossActionManager {
public:
	BossActionFirst();
	~BossActionFirst() = default;

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
};
