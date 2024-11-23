#include "BossActionOne.h"

#include "../BossBehavior/BaseBossBehavior.h"
#include "../BossBehavior/BossBehaviorStay.h"
#include "../BossBehavior/Rush/BossBehaviorRushStart.h"
#include "../BossBehavior/Rush/BossBehaviorRushPlay.h"
#include "../BossBehavior/Rush/BossBehaviorRushEnd.h"

BossActionOne::BossActionOne() {
	defaultAction = std::make_unique<ActionFlow>();
	defaultAction->flowName.emplace_back("Stay");

	auto& newFlow = flows.emplace_back();
	newFlow.flowName = { "RushStart", "RushPlay", "RushEnd" };
	newFlow.coolTime = 5.0f;
}

std::unique_ptr<BaseBossBehavior> BossActionOne::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(3.0f, playerManager);
	}
	else if (behaviorName == "RushStart") {
		return std::make_unique<BossBehaviorRushStart>(Quaternion::Identity(), 1.0f, 2.0f);
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(3.0f, 5.0f);
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}

	return nullptr;
}
