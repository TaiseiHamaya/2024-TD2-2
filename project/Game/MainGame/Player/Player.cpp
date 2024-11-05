#include "Player.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Input.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

void Player::initialize() {
	SetName("Player");

	ModelBehavior::model_ = SxavengerGame::LoadModel("Resources/model/CG2", "sphere.obj");
	model_->ApplyMeshShader();

	transform_.UpdateMatrix();

	renderingFlag_ = kBehaviorRender_Systematic;
}

void Player::input() {
	inputStick =
		SxavengerEngine::GetInput()->
		GetGamepadInput(0)->
		GetLStickNormalize();
	if (Length(inputStick) <= 0.1f) {
		inputStick = { 0.0f,0.0f };
	}
}

void Player::update() {
	Vector3f moveDirection = { inputStick.x, 0.0f, inputStick.y };
	transform_.transform.translate += moveDirection * Performance::GetDeltaTime(s).time;
}

void Player::update_matrix() {
	transform_.UpdateMatrix();

	worldPosition = transform_.GetWorldPosition();
}

const Vector3f& Player::GetColliderPosition() const {
	return worldPosition;
}
