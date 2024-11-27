#include "BossBehaviorTonguePlay.h"

#include "../../Boss.h"

#include <Lib/Easing.h>

BossBehaviorTonguePlay::BossBehaviorTonguePlay(float length, float Time_) :
	Time(Time_),
	startRotation(boss->get_transform().transform.rotate),
	colliderLength(length) {
	boss->set_model("enemy_tongue.gltf");
	attackCollider = std::make_unique<Collider>();
	attackCollider->SetTypeId(ColliderTypeBossAttack);
	attackCollider->SetTargetTypeId(ColliderTypePlayerHit);


	Vector3f forward = RotateVector({ 0,0,1 }, startRotation);
	apply_collider(forward);
	Sxavenger::PlayAudioOneShot("enemy_tongue.wav", 0.4f);
}

void BossBehaviorTonguePlay::move() {
	Quaternion& bossRotation = boss->get_transform().transform.rotate;
	// ボスの回転
	float parametric = std::min(1.0f, timer.time / Time);
	float angle = std::lerp(0.0f, pi_v * 2, EaseInOutBack(parametric));
	Quaternion rotationAngles = MakeAxisAngle(Vector3f{ 0,1,0 }, angle);
	bossRotation = startRotation * rotationAngles;

	// コリジョンの適用
	Vector3f forward = RotateVector({ 0,0,1 }, bossRotation);
	apply_collider(forward);

	if (timer.time >= Time) {
		isEnd = true;
	}
}

void BossBehaviorTonguePlay::update_collider(const Vector3f& position) {
	if (attackCollider) {
		Quaternion& bossRotation = boss->get_transform().transform.rotate;
		Vector3f forward = RotateVector({ 0,0,1 }, bossRotation);
		attackCollider->SetColliderPosition(position + forward * (colliderLength / 2));
	}
}

void BossBehaviorTonguePlay::apply_collider(const Vector3f& direction) {
	attackCollider->SetColliderBoundingCapsule(
		{
			.direction = direction,
			.radius = 2.0f,
			.length = colliderLength
		}
	);
}
