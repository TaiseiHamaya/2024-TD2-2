#include "BossBehaviorJumpStomp.h"

#include "../../Boss.h"

#include <Lib/Easing.h>

BossBehaviorJumpStomp::BossBehaviorJumpStomp(float StompTime_) :
	StompTime(StompTime_) {
	boss->set_model("enemy_stomp.gltf");
	attackCollider = std::make_unique<Collider>();
	attackCollider->SetTypeId(ColliderTypeBossAttack);
	attackCollider->SetTargetTypeId(ColliderTypePlayerHit);
	attackCollider->SetColliderBoundingSphere({.radius = 4.0f});
	StartHeight = boss->get_transform().transform.translate.y;
}

void BossBehaviorJumpStomp::move() {
	float parametric = std::min(1.0f, timer.time / StompTime);
	boss->get_transform().transform.translate.y = std::lerp(StartHeight, 0.0f, EaseOutQuad(EaseInOutBack(parametric)));

	if (timer >= boss->get_animator()->GetAnimationDuration(0)) {
		isEnd = true;
	}
}
