#include "Player.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Performance.h>

#include <Lib/Easing.h>

#include "PlayerState/PlayerStateGather.h"

void Player::initialize(const Vector3f& translate, float size_) {
	SetName("Player" + std::to_string(index));

	TryLoadJson("Player.json");

	hitCollider = std::make_unique<Collider>();
	hitCollider->SetColliderBoundingSphere();
	hitCollider->SetTypeId(ColliderType::ColliderTypePlayerHit);
	hitCollider->SetTargetTypeId(ColliderType::ColliderTypeBossAttack | ColliderType::ColliderTypePlayerHit);

	DefaultInvincibleTime.time = 3.0f;

	++index;

	set_model("player_move.gltf");

	set_sizing(size_);
	transform_.transform.translate = translate;
	update_matrix();

	renderingFlag_ = kBehaviorRender_Systematic;

	SetToConsole();
}

void Player::begin() {
	if (!stateQue.empty()) {
		stateQue.front()->begin();
	}
	velocity = kOrigin3;
	animationTimer.AddDeltaTime();

	invincibleTimer.SubtractDeltaTime();
	if (invincibleTimer.time <= 0) {
		invincibleTimer.time = 0.0f;
		isInvincible = false;
	}
}

void Player::update() {
	if (!stateQue.empty()) {
		if (stateQue.front()->is_end()) {
			stateQue.pop_front();
			if (stateQue.empty()) {
				set_model("player_move.gltf");
			}
			else {
				set_model(stateQue.front()->get_model_name());
			}
		}
	}

	if (!stateQue.empty()) {
		velocity = stateQue.front()->velocity();
	}
	Vector3f& translate = transform_.transform.translate;
	translate += velocity * Performance::GetDeltaTime(s).time;

	float wall = 40;
	if (Length(translate) > wall - size / 2) {
		translate = Normalize(translate) * (wall - size / 2);
		if (!stateQue.empty()) {
			stateQue.front()->end_force();
		}
	}

	if (translate.y < 0 || stateQue.empty()) {
		translate.y = 0;
	}

	color_.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (invincibleTimer.time > 0) {
		float t = invincibleTimer.time / DefaultInvincibleTime.time;

		if (static_cast<uint32_t>(t * kInvincibleDivisionNum_) % 2 == 0) {
			color_.color = { 12.0f, 12.0f, 12.0f, 1.0f };
		}
	}

	color_.Transfer();
}

void Player::update_matrix() {
	transform_.UpdateMatrix();

	Vector3f colliderPosition = transform_.GetWorldPosition();

	hitCollider->SetColliderPosition(colliderPosition);

	for (auto& queue : stateQue) {
		Collider* collider = queue->get_attack_collider().get();
		if (collider) {
			collider->SetColliderPosition(colliderPosition);
		}
	}

	for (uint32_t i = 0; i < animator_->GetAnimationSize(); ++i) {
		animator_->Update(animationTimer, i, true);
	}

	if (animationTimer > animator_->GetAnimationDuration(0)) {
		animationTimer.time = std::fmod(animationTimer.time, animator_->GetAnimationDuration(0).time);
		Sxavenger::PlayAudioOneShot("player_jump.wav", 0.2f);
	}
}

void Player::operate_update(const Vector2f& input) {
	if (!stateQue.empty()) {
		return;
	}
	Vector3f moveDirection = { input.x, 0.0f, input.y };
	velocity = moveDirection * MoveSpeed;

	if (Length(moveDirection) >= 0) {
		Quaternion lookAt = LookAt({ 0,0,1 }, Normalize(moveDirection));
		transform_.transform.rotate =
			Slerp(transform_.transform.rotate, lookAt, 0.5f);
	}
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

void Player::take_damage() {
	set_sizing(size - 0.5f);
	invincibleTimer = DefaultInvincibleTime;
	isInvincible = true;
}

Vector3f Player::world_point() const {
	return transform_.GetWorldPosition();
}

void Player::push_state(std::unique_ptr<BasePlayerState> state_) {
	if (stateQue.empty()) {
		set_model(state_->get_model_name());
	}
	stateQue.emplace_back(std::move(state_));
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

void Player::set_sizing(float size_) {
	size = size_;
	scaling = CreateScale(size);
	transform_.transform.scale = { scaling,scaling,scaling };
	hitCollider->SetColliderBoundingSphere({ .radius = scaling / 2 });
}

bool Player::is_invincible() const {
	if (isInvincible || get_attack_collider()) {
		return true;
	}
	return false;
}

struct AnimationModelP {
	Model* model;
	std::unique_ptr<Animator> animator;
};

void Player::set_model(const std::string& file) {
	static std::unordered_map<std::string, AnimationModelP> models;
	if (!models.contains(file)) {
		auto& newModels = models.emplace(file, AnimationModelP{}).first->second;
		newModels.model = SxavengerGame::LoadModel("ResourcesData/GameScene/Model", file);
		newModels.model->ApplyMeshShader();
		newModels.animator = std::make_unique<Animator>(newModels.model);
	}
	model_ = models[file].model;
	animator_ = models[file].animator.get();
}

void Player::SystemAttributeImGui() {
	ModelBehavior::SystemAttributeImGui();
	ImGui::Text("Size : %f", size);
	ImGui::Text("Scaling : %f", scaling);
	ImGui::DragFloat3("Velocity", &velocity.x, 0.1f);
}

float Player::CreateScale(float size) {
	float result = size / ModelSize;
	return result;
}
