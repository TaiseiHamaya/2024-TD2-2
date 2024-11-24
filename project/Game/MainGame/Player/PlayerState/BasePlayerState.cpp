#include "BasePlayerState.h"

#include <Engine/Game/Collider/Collider.h>

BasePlayerState::BasePlayerState() = default;

BasePlayerState::~BasePlayerState() = default;

void BasePlayerState::update_collider(const Vector3f& position) {
	if (attackCollider) {
		attackCollider->SetColliderPosition(position);
	}
}

void BasePlayerState::create_collider() {
	attackCollider = std::make_unique<Collider>();
	attackCollider->SetColliderBoundingSphere();
	attackCollider->SetTypeId(ColliderType::ColliderTypePlayerAttack);
	attackCollider->SetTargetTypeId(ColliderType::ColliderTypeBossHit);
}
