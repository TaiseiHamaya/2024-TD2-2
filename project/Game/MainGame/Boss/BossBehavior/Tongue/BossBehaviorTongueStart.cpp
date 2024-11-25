#include "BossBehaviorTongueStart.h"

#include "../../Boss.h"

BossBehaviorTongueStart::BossBehaviorTongueStart() {
	boss->set_model("enemy_tongue_start.gltf");
}

void BossBehaviorTongueStart::move() {
	auto duration = boss->get_animator()->GetAnimationDuration(0);
	if (duration <= timer) {
		timer = duration;
		isEnd = true;
	}
}
