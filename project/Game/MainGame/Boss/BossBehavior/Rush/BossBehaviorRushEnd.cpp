#include "BossBehaviorRushEnd.h"

#include "../../Boss.h"

BossBehaviorRushEnd::BossBehaviorRushEnd() {
	boss->set_model("enemy_rush_end.gltf");
}

void BossBehaviorRushEnd::move() {
	const Animator* const animator = boss->get_animator();
	if (animator->GetAnimationDuration(0) <= timer) {
		isEnd = true;
	}
}
