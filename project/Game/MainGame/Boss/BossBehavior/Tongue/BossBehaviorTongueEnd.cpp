#include "BossBehaviorTongueEnd.h"

#include "../../Boss.h"

BossBehaviorTongueEnd::BossBehaviorTongueEnd() {
	boss->set_model("enemy_tongue_end.gltf");
}

void BossBehaviorTongueEnd::move() {
	auto duration = boss->get_animator()->GetAnimationDuration(0);
	if (timer >= duration) {
		timer = duration;
		isEnd = true;
	}
}
