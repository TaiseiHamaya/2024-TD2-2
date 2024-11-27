#include "BossActionThird.h"

#include "../BossBehavior/BaseBossBehavior.h"
#include "../BossBehavior/BossBehaviorStay.h"
#include "../BossBehavior/Rush/BossBehaviorRushEnd.h"
#include "../BossBehavior/Rush/BossBehaviorRushPlay.h"
#include "../BossBehavior/Rush/BossBehaviorRushStart.h"
#include "../BossBehavior/Tongue/BossBehaviorTongueEnd.h"
#include "../BossBehavior/Tongue/BossBehaviorTonguePlay.h"
#include "../BossBehavior/Tongue/BossBehaviorTongueStart.h"

#include <Lib/MyMath.h>

BossActionThird::BossActionThird() {
	defaultAction = std::make_unique<ActionFlow>();
	defaultAction->flowName.emplace_back("Stay");

	auto& newFlow = actionFlows.emplace_back();
	newFlow.flowName = {
		"RushStartR", "RushPlay", "RushEnd",
		"RushStartL", "RushPlay", "RushEnd",
		"Stay",
		"TongueStart", "TonguePlay", "TongueEnd" };
	newFlow.coolTime = 5.0f;
	maxHitpoint = 10;
}

std::unique_ptr<BaseBossBehavior> BossActionThird::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(3.0f, playerManager); // 待機時間, 2つ目は必要なのでこれ
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, pi_v / 2), 1.0f, 2.0f);
	}
	else if (behaviorName == "RushStartL") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, -pi_v / 2), 1.0f, 2.0f); // 向き, 回転開始までの時間, 全体の時間
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(3.0f, 5.0f); // 突進長, 速度
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}
	else if (behaviorName == "TongueStart") {
		return std::make_unique<BossBehaviorTongueStart>(); // 
	}
	else if (behaviorName == "TonguePlay") {
		return std::make_unique<BossBehaviorTonguePlay>(20.0f, 1.0f); // コリジョンの長さ, 回転にかかる時間
	}
	else if (behaviorName == "TongueEnd") {
		return std::make_unique<BossBehaviorTongueEnd>(); // 
	}

	return nullptr;
}
