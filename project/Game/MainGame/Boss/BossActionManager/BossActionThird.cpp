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
		"TongueStart", "TonguePlay", "TongueEnd",
		"Stay"
	};
	newFlow.coolTime = 5.0f;
	maxHitpoint = 10;

	SetName("BossActionThird");
	exporter_.TryLoadFromJson();

	exporter_.GetFromStash("StayTime", &StayTime, 1);
	exporter_.GetFromStash("LookAtStartTime", &LookAtStartTime, 1);
	exporter_.GetFromStash("RushStartTime", &RushStartTime, 1);
	exporter_.GetFromStash("RushSpeed", &RushSpeed, 1);
	exporter_.GetFromStash("RushLength", &RushLength, 1);
	exporter_.GetFromStash("ColliderLength", &ColliderLength, 1);
	exporter_.GetFromStash("TurnTime", &TurnTime, 1);

	SetToConsole();
}

std::unique_ptr<BaseBossBehavior> BossActionThird::create(const std::string& behaviorName) {
	if (behaviorName == "Stay") {
		return std::make_unique<BossBehaviorStay>(StayTime, playerManager); // 待機時間, 2つ目は必要なのでこれ
	}
	else if (behaviorName == "RushStartR") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, pi_v / 2), LookAtStartTime, RushStartTime);
	}
	else if (behaviorName == "RushStartL") {
		return std::make_unique<BossBehaviorRushStart>(
			MakeAxisAngle({ 0,1,0 }, -pi_v / 2), LookAtStartTime, RushStartTime); // 向き, 回転開始までの時間, 全体の時間
	}
	else if (behaviorName == "RushPlay") {
		return std::make_unique<BossBehaviorRushPlay>(RushLength, RushSpeed); // 突進長, 速度
	}
	else if (behaviorName == "RushEnd") {
		return std::make_unique<BossBehaviorRushEnd>();
	}
	else if (behaviorName == "TongueStart") {
		return std::make_unique<BossBehaviorTongueStart>(); // 
	}
	else if (behaviorName == "TonguePlay") {
		return std::make_unique<BossBehaviorTonguePlay>(ColliderLength, TurnTime); // コリジョンの長さ, 回転にかかる時間
	}
	else if (behaviorName == "TongueEnd") {
		return std::make_unique<BossBehaviorTongueEnd>(); // 
	}

	return nullptr;
}

void BossActionThird::SetAttributeImGui() {
	exporter_.DragFloat("StayTime", &StayTime, 0.1f);
	exporter_.DragFloat("LookAtStartTime", &LookAtStartTime, 0.1f);
	exporter_.DragFloat("RushStartTime", &RushStartTime, 0.1f);
	exporter_.DragFloat("RushSpeed", &RushSpeed, 0.1f);
	exporter_.DragFloat("RushLength", &RushLength, 0.1f);
	exporter_.DragFloat("ColliderLength", &ColliderLength, 0.1f);
	exporter_.DragFloat("TurnTime", &TurnTime, 0.1f);

	//exporter_.("RushLength", &RushLength, 0.1f);

	if (ImGui::Button("Export")) {
		exporter_.OutputToJson();
	}
}
