#include "BossBehaviorRushPlay.h"

#include <Lib/Geometry/Quaternion.h>

#include "../../Boss.h"

BossBehaviorRushPlay::BossBehaviorRushPlay(float RushLength_, float RushSpeed_) :
	direction(RotateVector(Vector3f{0.0f,0.0f,1.0f}, boss->get_transform().transform.rotate)),
	RushLength(RushLength_), RushSpeed(RushSpeed_) {
	rushAmount = 0;
	boss->set_model("enemy_rush.gltf");
	attackCollider = std::make_unique<Collider>();
	attackCollider->SetTypeId(ColliderTypeBossAttack);
	attackCollider->SetTargetTypeId(ColliderTypeBossAttack);
	attackCollider->SetColliderBoundingSphere();
}

void BossBehaviorRushPlay::move() {
	// 移動処理
	boss->get_transform().transform.translate += direction * RushSpeed * Performance::GetDeltaTime(s).time;
	rushAmount += RushSpeed * Performance::GetDeltaTime(s).time;

	// 終了条件
	if (rushAmount >= RushLength || isHitCollision) {
		isEnd = true;
	}
}
