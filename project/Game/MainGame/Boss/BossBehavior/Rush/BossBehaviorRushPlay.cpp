#include "BossBehaviorRushPlay.h"

#include <Lib/Geometry/Quaternion.h>
#include <Lib/Adapter/Random/Random.h>

#include "../../Boss.h"

BossBehaviorRushPlay::BossBehaviorRushPlay(float RushLength_, float RushSpeed_) :
	direction(RotateVector(Vector3f{0.0f,0.0f,1.0f}, boss->get_transform().transform.rotate)),
	RushLength(RushLength_), RushSpeed(RushSpeed_) {
	rushAmount = 0;
	boss->set_model("enemy_rush.gltf");
	attackCollider = std::make_unique<Collider>();
	attackCollider->SetTypeId(ColliderTypeBossAttack);
	attackCollider->SetTargetTypeId(ColliderTypePlayerHit);
	attackCollider->SetColliderBoundingSphere({ .radius = 3.0f });
}

void BossBehaviorRushPlay::move() {
	// 移動処理
	boss->get_transform().transform.translate += direction * RushSpeed * Performance::GetDeltaTime(s).time;
	rushAmount += RushSpeed * Performance::GetDeltaTime(s).time;

	// 終了条件
	if (rushAmount >= RushLength || isHitCollision) {
		isEnd = true;
	}

	if (animationDurationCount_ < static_cast<uint32_t>(timer.time / (boss->get_animator()->GetAnimationDuration(0).time / 4.0f))) {
		animationDurationCount_ = static_cast<uint32_t>(timer.time / (boss->get_animator()->GetAnimationDuration(0).time / 4.0f));
		RushEffect();
	}
}

void BossBehaviorRushPlay::RushEffect() {
	for (uint32_t i = 0; i < 6; ++i) {

		Vector3f velocity = -direction * Random::Generate(12.0f, 24.0f);
		Quaternion random = MakeAxisAngle({ 0.0f, 1.0f, 0.0f }, Random::Generate(-pi_v / 8.0f, pi_v / 8.0f));


		boss->CreateLandingParticle(RotateVector(velocity, random));
	}
}
