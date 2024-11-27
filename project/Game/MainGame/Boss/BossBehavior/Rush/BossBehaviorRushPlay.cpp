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
	Sxavenger::PlayAudioOneShot("enemy_bite.wav", 0.2f);
}

void BossBehaviorRushPlay::move() {
	Vector3f& translate = boss->get_transform().transform.translate;
	// 移動処理
	translate += direction * RushSpeed * Performance::GetDeltaTime(s).time;
	rushAmount += RushSpeed * Performance::GetDeltaTime(s).time;

	// 終了条件
	float length = Length(translate);
	if (rushAmount >= RushLength || isHitCollision || length >= 38.0f) {
		if (length >= 38.0f) {
			Vector3f normalized = Normalize(translate);
			translate = normalized * 37.0f;
		}
		isEnd = true;
	}

	if (animationDurationCount_ < static_cast<uint32_t>(timer.time / (boss->get_animator()->GetAnimationDuration(0).time / 4.0f))) {
		animationDurationCount_ = static_cast<uint32_t>(timer.time / (boss->get_animator()->GetAnimationDuration(0).time / 4.0f));
		RushEffect();
		Sxavenger::PlayAudioOneShot("enemy_bite.wav", 0.2f);
	}
}

void BossBehaviorRushPlay::RushEffect() {
	for (uint32_t i = 0; i < 6; ++i) {

		Vector3f velocity = -direction * Random::Generate(12.0f, 24.0f);
		Quaternion random = MakeAxisAngle({ 0.0f, 1.0f, 0.0f }, Random::Generate(-pi_v / 8.0f, pi_v / 8.0f));


		boss->CreateLandingParticle(RotateVector(velocity, random));
	}
}
