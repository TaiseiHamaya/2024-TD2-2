#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

#include "BossBehavior/BaseBossBehavior.h"

Boss::Boss(int32_t hitpoint_) :
	hitpoint(hitpoint_) {
	SetName("Boss");
	// モデル設定
	
	transform_.UpdateMatrix();
	renderingFlag_ = kBehaviorRender_Systematic;

	TryLoadJson();

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();
	collider_->SetTypeId(ColliderType::ColliderTypeBossHit);
	collider_->SetTargetTypeId(ColliderType::ColliderTypePlayerAttack);

	exporter_.TryLoadFromJson();
}

Boss::~Boss() noexcept {
	finalize();
}

void Boss::begin() {
	damagedInvincibleTimer.SubtractDeltaTime();
	if (damagedInvincibleTimer.time <= 0) {
		isInvincible = false;
	}
}

void Boss::update() {
	if (behavior) {
		behavior->begin();
		behavior->move();
	}
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
	collider_->SetColliderPosition(transform_.GetWorldPosition());
	// アニメーション更新
	if (!animator) {
		return;
	}
	//for (uint32_t i = 0; i < animator->GetAnimationSize(); ++i) {
	//	animator->Update(behavior->get_timer() , i, true);
	//}
	animator->Update(behavior->get_timer(), 0, true);

	if (behavior) {
		behavior->update_collider(transform_.GetWorldPosition());
	}
}

void Boss::finalize() {
	exporter_.OutputToJson();
}

void Boss::SetAttributeImGui() {
	ImGui::Text("%f", behavior->get_timer().time);
	ImGui::Text("%d", hitpoint);
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
	animator = models[file].animator.get();
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
