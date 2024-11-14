#include "Player.h"

#include <Engine/Game/SxavengerGame.h>

#include <Engine/System/Performance.h>

void Player::initialize() {
	SetName("Player" + std::to_string(index));
	++index;

	model_ = SxavengerGame::LoadModel("Resources/model/CG2", "sphere.obj");
	model_->ApplyMeshShader();

	transform_.UpdateMatrix();

	renderingFlag_ = kBehaviorRender_Systematic;

	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();

	SetToConsole();
}

void Player::begin() {
	if (state) {
		state->begin();
	}
}

void Player::update() {
	if (state) {
		velocity = state->velocity();
	}
	transform_.transform.translate += velocity * Performance::GetDeltaTime(s).time;
}

void Player::update_matrix() {
	transform_.UpdateMatrix();

	collider_->SetColliderPosition(transform_.GetWorldPosition());
}

void Player::operate_update(const Vector2f& input) {
	if (state) {
		return;
	}
	Vector3f moveDirection = { input.x, 0.0f, input.y };
	velocity = moveDirection * 3.0f;
}
