#include "BossBehaviorJumpStomp.h"

#include "../../Boss.h"

#include <Lib/Easing.h>
#include <Lib/Adapter/Random/Random.h>

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
		CreateStompEffect();
	}
}

void BossBehaviorJumpStomp::CreateStompEffect() {
	for (uint32_t i = 0; i < 12; ++i) {

		Vector3f velocity = { 0.0f, Random::Generate(0.01f, 2.0f), Random::Generate(12.0f, 24.0f) };
		Quaternion random = MakeAxisAngle({ 0.0f, 1.0f, 0.0f, }, Random::Generate(0.0f, pi_v * 2.0f));

		boss->CreateLandingParticle(RotateVector(velocity, random));
	}
}
