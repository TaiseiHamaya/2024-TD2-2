#include "BossBehaviorJumpFly.h"

#include "../../Boss.h"

#include "Game/MainGame/Player/Player.h"
#include "Game/MainGame/Player/PlayerManager.h"

BossBehaviorJumpFly::BossBehaviorJumpFly(float FlyTime_, float ApproachTime_, float ApproachSpeed_, const PlayerManager* playerManager_) :
	FlyTime(FlyTime_),
	ApproachTime(ApproachTime_),
	ApproachSpeed(ApproachSpeed_),
	playerManager(playerManager_) {
	boss->set_model("enemy_fly.gltf");
}

void BossBehaviorJumpFly::move() {
	if (timer.time < ApproachTime) {
		const Player* player = playerManager->get_operate_player();
		Vector3f target = player->world_point();
		Vector3f begin = boss->get_transform().GetWorldPosition();

		target.y = begin.y;

		Vector3f direction = All(target == begin) ? kOrigin3 : Normalize(target - begin);
		Vector3f velocity = direction * ApproachSpeed;

		boss->get_transform().transform.translate +=
			velocity * Performance::GetDeltaTime(s).time;
	}

	if (timer.time >= FlyTime) {
		isEnd = true;
	}
}
