#include "BossActionFirst.h"

#include "../BossBehavior/BaseBossBehavior.h"
#include "../BossBehavior/BossBehaviorStay.h"
#include "../BossBehavior/Rush/BossBehaviorRushStart.h"
#include "../BossBehavior/Rush/BossBehaviorRushPlay.h"
#include "../BossBehavior/Rush/BossBehaviorRushEnd.h"

#include <Lib/MyMath.h>

BossActionFirst::BossActionFirst() {
	defaultAction = std::make_unique<ActionFlow>();
	defaultAction->flowName.emplace_back("Stay");

	auto& newFlow = actionFlows.emplace_back();
	newFlow.flowName = { "RushStartR", "RushPlay", "RushEnd","RushStartB", "RushPlay", "RushEnd","RushStartR", "RushPlay", "RushEnd", "Stay" };
	newFlow.coolTime = 5.0f;
	maxHitpoint = 3;
}

std::unique_ptr<BaseBossBehavior> BossActionFirst::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(3.0f, playerManager);
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, pi_v / 2), 1.0f, 2.0f);
	}
	else if (behaviorName == "RushStartB") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, pi_v), 1.0f, 2.0f);
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(3.0f, 5.0f);
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}

	return nullptr;
}