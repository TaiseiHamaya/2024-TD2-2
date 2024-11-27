#include "BossBehaviorAnimation.h"

#include "../Boss.h"

BossBehaviorAnimation::BossBehaviorAnimation(const std::string& animationName) {
	boss->set_model(animationName);
}

void BossBehaviorAnimation::move() {
	auto duration = boss->get_animator()->GetAnimationDuration(0);
	if (duration <= timer) {
		timer = duration;
		isEnd = true;
	}
}
