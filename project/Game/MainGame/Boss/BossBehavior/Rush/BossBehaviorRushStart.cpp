#include "BossBehaviorRushStart.h"

#include "../../Boss.h"

BossBehaviorRushStart::BossBehaviorRushStart(const  Quaternion& rushDirection, float LookStartTime_, float RushStartTime_) :
	RushStartTime(RushStartTime_), LookStartTime(LookStartTime_),
	beginRotation(
		boss->get_transform().transform.rotate
	),
	endRotation(beginRotation * rushDirection) {
	boss->set_model("enemy_rush_start.gltf");
}

void BossBehaviorRushStart::move() {
	if (timer.time >= LookStartTime) {
		float whole = RushStartTime - LookStartTime;
		float now = timer.time - LookStartTime;
		boss->get_transform().transform.rotate =
			Slerp(beginRotation, endRotation, std::min(1.0f, now / whole));
	}

	if (timer.time >= RushStartTime) {
		isEnd = true;
	}
}
