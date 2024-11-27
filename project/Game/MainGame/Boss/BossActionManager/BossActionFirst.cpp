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
	newFlow.flowName = {
		"RushStartR", "RushPlay", "RushEnd",
		"StayShort",
		"RushStartB", "RushPlay", "RushEnd",
		"StayShort",
		"RushStartR", "RushPlay", "RushEnd",
		"Stay" };
	newFlow.coolTime = 1.0f;
	maxHitpoint = 3;

	SetName("BossActionFirst");
	exporter_.TryLoadFromJson();

	exporter_.GetFromStash("StayTime", &StayTime, 1);
	exporter_.GetFromStash("LookAtStartTime", &LookAtStartTime, 1);
	exporter_.GetFromStash("RushStartTime", &RushStartTime, 1);
	exporter_.GetFromStash("RushSpeed", &RushSpeed, 1);
	exporter_.GetFromStash("RushLength", &RushLength, 1);

	SetToConsole();
}

std::unique_ptr<BaseBossBehavior> BossActionFirst::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(StayTime, playerManager);
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, pi_v / 2), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushStartB") {
		return std::make_unique<BossBehaviorRushStart>(MakeAxisAngle({ 0,1,0 }, pi_v), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(RushLength, RushSpeed);
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}
	else if (behaviorName == "StayShort") {
		return std::make_unique<BossBehaviorStay>(0.1f, playerManager);
	}

	return nullptr;
}

#ifdef _DEBUG
void BossActionFirst::SetAttributeImGui() {
	exporter_.DragFloat("StayTime", &StayTime, 0.1f);
	exporter_.DragFloat("LookAtStartTime", &LookAtStartTime, 0.1f);
	exporter_.DragFloat("RushStartTime", &RushStartTime, 0.1f);
	exporter_.DragFloat("RushSpeed", &RushSpeed, 0.1f);
	exporter_.DragFloat("RushLength", &RushLength, 0.1f);

	//exporter_.("RushLength", &RushLength, 0.1f);

	if (ImGui::Button("Export")) {
		exporter_.OutputToJson();
	}
}

#endif // DEBUG