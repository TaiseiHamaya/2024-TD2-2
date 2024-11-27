#include "PlayerStateGather.h"

#include <Engine/Game/Transform.h>

#include "../PlayerManager.h"

PlayerState::Gather::Gather(const QuaternionTransformBuffer* thisTransform_) :
	thisTransform(thisTransform_) {
	create_collider();
}

void PlayerState::Gather::begin() {
}

Vector3f PlayerState::Gather::velocity() {
	Vector3f targetPosition = playerManager->get_operate_player()->get_transform().GetWorldPosition();
	Vector3f direction = targetPosition - thisTransform->GetWorldPosition();
	direction = Normalize(direction);
	return direction * 8.0f;
}
