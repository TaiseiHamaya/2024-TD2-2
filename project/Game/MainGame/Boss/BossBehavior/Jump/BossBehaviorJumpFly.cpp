#include "BossBehaviorJumpFly.h"

#include "../../Boss.h"

#include "Game/MainGame/Player/Player.h"
#include "Game/MainGame/Player/PlayerManager.h"

BossBehaviorJumpFly::BossBehaviorJumpFly(float FlyTime_, float ApproachTime_, const PlayerManager* playerManager_) :
	FlyTime(FlyTime_),
	ApproachTime(ApproachTime_),
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
		float SPEED = 3.0f;

		boss->get_transform().transform.translate += direction * SPEED * Performance::GetDeltaTime(s).time;
	}

	if (timer.time >= FlyTime) {
		isEnd = true;
	}
}
