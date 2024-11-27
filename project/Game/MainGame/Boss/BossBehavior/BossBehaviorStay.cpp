#include "BossBehaviorStay.h"

#include "../Boss.h"

#include "Game/MainGame/Player/Player.h"
#include "Game/MainGame/Player/PlayerManager.h"

#include <Lib/Adapter/Random/Random.h>

BossBehaviorStay::BossBehaviorStay(float Time, const PlayerManager* playerManager_) :
	stayTime(Time), playerManager(playerManager_) {
	boss->set_model("enemy_stay.gltf");
}

void BossBehaviorStay::move() {
	auto& transform = boss->get_transform().transform;
	Vector3f forward = RotateVector({ 0,0,1 }, transform.rotate);
	// プレイヤー方向を向く処理
	if (playerManager) {
		const Player* player = playerManager->get_operate_player();
		Vector3f target = player->get_transform().GetWorldPosition() - boss->get_transform().GetWorldPosition();
		if (Length(target) >= 0.1f) {
			target = Normalize(target);
			transform.rotate *= Slerp(Quaternion::Identity(), LookAt(forward, target), 0.1f);
		}
	}
	//終了したか判定
	if (timer.time >= stayTime) {
		isEnd = true;
	}

	uint32_t AnimationParametric = static_cast<uint32_t>(timer.time / boss->get_animator()->GetAnimationDuration(0).time);
	if (animationDurationCount_ < AnimationParametric) {
		animationDurationCount_ = AnimationParametric;
		CreateStayEffect();
	}
}

void BossBehaviorStay::CreateStayEffect() {
	for (uint32_t i = 0; i < 4; ++i) {

		Vector3f velocity = { 0.0f, Random::Generate(0.01f, 2.0f), Random::Generate(12.0f, 24.0f) };
		Quaternion random = MakeAxisAngle({ 0.0f, 1.0f, 0.0f, }, Random::Generate(0.0f, pi_v * 2.0f));

		boss->CreateLandingParticle(RotateVector(velocity, random));
	}
}
