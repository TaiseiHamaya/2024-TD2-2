#include "PlayerStateGather.h"

#include <Engine/Game/Transform.h>

PlayerState::Gather::Gather(const QuaternionTransformBuffer* thisTransform_, const QuaternionTransformBuffer* target_) :
	thisTransform(thisTransform_), target(target_) {
}

void PlayerState::Gather::begin() {
}

Vector3f PlayerState::Gather::velocity() {
	Vector3f direction = target->GetWorldPosition() - thisTransform->GetWorldPosition();
	direction = Normalize(direction);
	return direction * 5.0f;
}
