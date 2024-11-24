#include "Player.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Performance.h>

#include "PlayerState/PlayerStateGather.h"

void Player::initialize(const Vector3f& translate, float scaling_) {
	SetName("Player" + std::to_string(index));

	hitCollider = std::make_unique<Collider>();
	hitCollider->SetColliderBoundingSphere();
	hitCollider->SetTypeId(ColliderType::ColliderTypePlayerHit);
	hitCollider->SetTargetTypeId(ColliderType::ColliderTypeBossAttack | ColliderType::ColliderTypePlayerHit);

	++index;

	model_ = SxavengerGame::LoadModel("Resources/model/CG2", "sphere.obj");
	model_->ApplyMeshShader();

	set_scaling(scaling_);
	transform_.transform.translate = translate;
	transform_.UpdateMatrix();

	renderingFlag_ = kBehaviorRender_Systematic;

	SetToConsole();

	update_matrix();
}

void Player::begin() {
	if (!stateQue.empty()) {
		stateQue.front()->begin();
	}
	velocity = kOrigin3;
}

void Player::update() {
	if (!stateQue.empty()) {
		if (stateQue.front()->is_end()) {
			stateQue.pop_front();
		}
	}

	if (!stateQue.empty()) {
		velocity = stateQue.front()->velocity();
	}
	transform_.transform.translate += velocity * Performance::GetDeltaTime(s).time;
}

void Player::update_matrix() {
	transform_.UpdateMatrix();

	hitCollider->SetColliderPosition(transform_.GetWorldPosition());

	if (!stateQue.empty()) {
		stateQue.front()->update_collider(transform_.GetWorldPosition());
	}
}

void Player::operate_update(const Vector2f& input) {
	if (!stateQue.empty()) {
		return;
	}
	Vector3f moveDirection = { input.x, 0.0f, input.y };
	velocity = moveDirection * 3.0f;
}

void Player::ungather() {
	constexpr auto& gatherType = typeid(PlayerState::Gather);
	for (auto itr = stateQue.begin(); itr != stateQue.end(); ) {
		const auto& itrType = typeid(*itr->get());
		if (itrType == gatherType) {
			itr = stateQue.erase(itr);
		}
		else {
			++itr;
		}
	}
}

Vector3f Player::world_point() const {
	return transform_.GetWorldPosition();
}

bool Player::empty_state() {
	return stateQue.empty();
}

Collider* Player::get_attack_collider() const {
	if (stateQue.empty()) {
		return nullptr;
	}
	else {
		auto& front = stateQue.front();
		return front->get_attack_collider().get();
	}
}

void Player::set_scaling(float scale_) {
	scaling = scale_;
	transform_.transform.scale = { scaling,scaling,scaling };
	hitCollider->SetColliderBoundingSphere({ .radius = scaling });
}

void Player::SystemAttributeImGui() {
	ModelBehavior::SystemAttributeImGui();
	ImGui::DragFloat3("Velocity", &velocity.x, 0.1f);
}
