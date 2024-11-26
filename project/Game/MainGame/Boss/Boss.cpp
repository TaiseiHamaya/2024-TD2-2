#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>
#include <Lib/Easing.h>
#include <Lib/Adapter/Random/Random.h>

#include "BossBehavior/BaseBossBehavior.h"
#include "Game/MainGame/Camera/GameCamera.h"

Boss::Boss(int32_t hitpoint_) :
	hitpoint(hitpoint_) {
	SetName("Boss");
	// モデル設定

	transform_.UpdateMatrix();
	renderingFlag_ = kBehaviorRender_Systematic;

	TryLoadJson();

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere({ .radius = 3.0f });
	collider_->SetTypeId(ColliderType::ColliderTypeBossHit);
	collider_->SetTargetTypeId(ColliderType::ColliderTypePlayerAttack);

	exporter_.TryLoadFromJson();

	landing_ = std::make_unique<EffectLanding>();
	landing_->Init();
	SetChild(landing_.get());

	shadow_ = std::make_unique<Shadow>();
	shadow_->Init();
	SetChild(shadow_.get());
}

Boss::~Boss() noexcept {
	finalize();
}

void Boss::begin() {
	damagedInvincibleTimer.SubtractDeltaTime();
	if (damagedInvincibleTimer.time <= 0.0f) {
		damagedInvincibleTimer.time = 0.0f;
		isInvincible = false;
	}
	if (behavior) {
		behavior->begin();
	}
}

void Boss::update() {
	if (behavior) {
		behavior->move();
	}

	landing_->Update();
	HitReactionUpdate();
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
	collider_->SetColliderPosition(transform_.GetWorldPosition());
	// アニメーション更新
	if (!animator_) {
		return;
	}
	for (uint32_t i = 0; i < animator_->GetAnimationSize(); ++i) {
		animator_->Update(behavior->get_timer() , i, true);
	}
	//animator_->Update(behavior->get_timer(), 0, true);

	if (behavior) {
		behavior->update_collider(transform_.GetWorldPosition());
	}

	shadow_->Update(0.0f, transform_.GetWorldPosition());
}

void Boss::finalize() {
	exporter_.OutputToJson();
}

void Boss::SetAttributeImGui() {
	ImGui::Text("%f", behavior->get_timer().time);
	ImGui::Text("%d", hitpoint);
}

void Boss::CreateLandingParticle(const Vector3f& velocity) {
	landing_->CreateParticle(transform_.GetWorldPosition(), velocity);
}

void Boss::HitReactionUpdate() {
	float t = EaseOutExpo(1.0f - damagedInvincibleTimer.time / 1.0f);

	color_.color.g = t;
	color_.color.b = t;
	color_.Transfer();
}

void Boss::ShakeCamera(DeltaTimePoint time, int32_t subdivision, float strength) {
	if (camera_ == nullptr) {
		return;
	}

	camera_->SetShake(time, subdivision, strength);
}

bool Boss::is_end_behavior() const {
	return behavior->is_end();
}

void Boss::set_behavior(std::unique_ptr<BaseBossBehavior> behavior_) {
	behavior = std::move(behavior_);
}

void Boss::take_damage(int32_t damage) {
	hitpoint -= damage;
}

struct AnimationModel {
	Model* model;
	std::unique_ptr<Animator> animator;
};

void Boss::set_model(const std::string& file) {
	static std::unordered_map<std::string, AnimationModel> models;
	if (!models.contains(file)) {
		auto& newModels = models.emplace(file, AnimationModel{}).first->second;
		newModels.model = SxavengerGame::LoadModel("ResourcesData/GameScene/Model", file);
		newModels.model->ApplyMeshShader();
		newModels.animator = std::make_unique<Animator>(newModels.model);
	}
	model_ = models[file].model;
	animator_ = models[file].animator.get();
}

bool Boss::is_dead() const {
	return hitpoint <= 0;
}

bool Boss::is_invincible() const {
	return isInvincible;
}

bool Boss::is_destroy() const {
	return isDestroy;
}

Collider* Boss::get_hit_collider() const {
	return collider_.get();
}

Collider* Boss::get_attack_collider() const {
	if (behavior) {
		return behavior->get_attack_collider().get();
	}
	return nullptr;
}

void Boss::hit_callback() {
	isInvincible = true;
	damagedInvincibleTimer.time = 1.0f;
}
