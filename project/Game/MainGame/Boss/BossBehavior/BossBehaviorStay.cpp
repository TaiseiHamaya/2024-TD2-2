#include "BossBehaviorStay.h"

#include "../Boss.h"

#include "Game/MainGame/Player/Player.h"
#include "Game/MainGame/Player/PlayerManager.h"

BossBehaviorStay::BossBehaviorStay(float Time, const PlayerManager* playerManager_) :
	stayTime(Time), playerManager(playerManager_) {
	boss->set_model("enemy_stay.gltf");
}

void BossBehaviorStay::move() {
	auto& transform = boss->get_transform().transform;
	Vector3f forward = RotateVector({ 0,0,1 }, transform.rotate);
	const Player* player = playerManager->get_operate_player();
	Vector3f target =  player->get_transform().GetWorldPosition()- boss->get_transform().GetWorldPosition();
	if (Length(target) >= 0.1f) {
		target = Normalize(target);
		transform.rotate *= Slerp(Quaternion::Identity(), LookAt(forward, target), 0.1f);
	}
	if (timer.time >= stayTime) {
		isEnd = true;
	}
}
