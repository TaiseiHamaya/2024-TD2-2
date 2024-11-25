#include "BossActionSecond.h"

#include "../BossBehavior/BaseBossBehavior.h"
#include "../BossBehavior/BossBehaviorStay.h"
#include "../BossBehavior/Rush/BossBehaviorRushStart.h"
#include "../BossBehavior/Rush/BossBehaviorRushPlay.h"
#include "../BossBehavior/Rush/BossBehaviorRushEnd.h"
#include "../BossBehavior/Jump/BossBehaviorJumpJump.h"
#include "../BossBehavior/Jump/BossBehaviorJumpFly.h"
#include "../BossBehavior/Jump/BossBehaviorJumpStomp.h"

#include <Lib/MyMath.h>

BossActionSecond::BossActionSecond() {
	defaultAction = std::make_unique<ActionFlow>();
	defaultAction->flowName.emplace_back("Stay");

	auto& newFlow = flows.emplace_back();
	newFlow.flowName = {
		"RushStartR", "RushPlay", "RushEnd",
		"RushStartB", "RushPlay", "RushEnd",
		"RushStartR", "RushPlay", "RushEnd",
		"Stay",
		"JumpJump", "JumpFly", "JumpStomp"};
	newFlow.coolTime = 5.0f;
}

std::unique_ptr<BaseBossBehavior> BossActionSecond::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(3.0f, playerManager); // 待機時間, 2つ目は必要なのでこれ
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, pi_v / 2), 1.0f, 2.0f);
	}
	else if (behaviorName == "RushStartB") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, pi_v), 1.0f, 2.0f); // 向き, 回転開始までの時間, 全体の時間
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(3.0f, 5.0f); // 突進長, 速度
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}
	else if (behaviorName == "JumpJump") {
		return std::make_unique<BossBehaviorJumpJump>(10.0f); // 高さ
	}
	else if (behaviorName == "JumpFly") {
		return std::make_unique<BossBehaviorJumpFly>(
			4.0f, 3.0f, playerManager); // 時間, 接近時間
	}
	else if (behaviorName == "JumpStomp") {
		return std::make_unique<BossBehaviorJumpStomp>(1.0f); // 落下時間
	}

	return nullptr;
}
