#include "BossActionSecond.h"

#include "../BossBehavior/BaseBossBehavior.h"
#include "../BossBehavior/BossBehaviorStay.h"
#include "../BossBehavior/Jump/BossBehaviorJumpFly.h"
#include "../BossBehavior/Jump/BossBehaviorJumpJump.h"
#include "../BossBehavior/Jump/BossBehaviorJumpStomp.h"
#include "../BossBehavior/Rush/BossBehaviorRushEnd.h"
#include "../BossBehavior/Rush/BossBehaviorRushPlay.h"
#include "../BossBehavior/Rush/BossBehaviorRushStart.h"

#include <Lib/MyMath.h>

BossActionSecond::BossActionSecond() {
	defaultAction = std::make_unique<ActionFlow>();
	defaultAction->flowName.emplace_back("Stay");

	auto& newFlow = actionFlows.emplace_back();
	newFlow.flowName = {
		"RushStartR", "RushPlay", "RushEnd",
		"StayShort",
		"RushStartL", "RushPlay", "RushEnd",
		"StayShort",
		"RushStartF", "RushPlay", "RushEnd",
		"Stay",
		"JumpJump", "JumpFly", "JumpStomp"};
	newFlow.coolTime = 5.0f;
	maxHitpoint = 7;

	SetName("BossActionSecond");
	exporter_.TryLoadFromJson();

	exporter_.GetFromStash("StayTime", &StayTime, 1);

	exporter_.GetFromStash("LookAtStartTime", &LookAtStartTime, 1);
	exporter_.GetFromStash("RushStartTime", &RushStartTime, 1);
	exporter_.GetFromStash("RushSpeed", &RushSpeed, 1);
	exporter_.GetFromStash("RushLength", &RushLength, 1);

	exporter_.GetFromStash("JumpHeight", &JumpHeight, 1);
	exporter_.GetFromStash("FlyingTime", &FlyingTime, 1);
	exporter_.GetFromStash("ApproachTime", &ApproachTime, 1);
	exporter_.GetFromStash("ApproachSpeed", &ApproachSpeed, 1);
	exporter_.GetFromStash("StompTime", &StompTime, 1);

	SetToConsole();
}

std::unique_ptr<BaseBossBehavior> BossActionSecond::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(StayTime, playerManager); // 待機時間, 2つ目は必要なのでこれ
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, pi_v / 2), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushStartF") {
		return std::make_unique<BossBehaviorRushStart>(Quaternion::Identity(), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushStartL") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, -pi_v / 2), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(RushLength, RushSpeed); // 突進長, 速度
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}
	else if (behaviorName == "JumpJump") {
		return std::make_unique<BossBehaviorJumpJump>(JumpHeight); // 高さ
	}
	else if (behaviorName == "JumpFly") {
		return std::make_unique<BossBehaviorJumpFly>(
			FlyingTime, ApproachTime, ApproachSpeed, playerManager); // 時間, 接近時間
	}
	else if (behaviorName == "JumpStomp") {
		return std::make_unique<BossBehaviorJumpStomp>(StompTime); // 落下時間
	}
	else if (behaviorName == "StayShort") {
		return std::make_unique<BossBehaviorStay>(0.1f, playerManager);
	}

	return nullptr;
}

#ifdef _DEBUG

void BossActionSecond::SetAttributeImGui() {
	exporter_.DragFloat("StayTime", &StayTime, 0.1f);
	exporter_.DragFloat("LookAtStartTime", &LookAtStartTime, 0.1f);
	exporter_.DragFloat("RushStartTime", &RushStartTime, 0.1f);
	exporter_.DragFloat("RushSpeed", &RushSpeed, 0.1f);
	exporter_.DragFloat("RushLength", &RushLength, 0.1f);

	exporter_.DragFloat("JumpHeight", &JumpHeight, 0.1f);
	exporter_.DragFloat("FlyingTime", &FlyingTime, 0.1f);
	exporter_.DragFloat("ApproachTime", &ApproachTime, 0.1f);
	exporter_.DragFloat("ApproachSpeed", &ApproachSpeed, 0.1f);
	exporter_.DragFloat("StompTime", &StompTime, 0.1f);

	if (ImGui::Button("Export")) {
		exporter_.OutputToJson();
	}
}

#endif // DEBUG
