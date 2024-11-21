#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

Boss::Boss(int32_t hitpoint_) :
	hitpoint(hitpoint_) {
	SetName("Boss");
	// モデル設定
	transform_.UpdateMatrix();
	set_model("sphere.obj");
	renderingFlag_ = kBehaviorRender_Systematic;

	TryLoadJson();

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();
	collider_->SetTypeId(ColliderType::ColliderTypeBoss);
	collider_->SetTargetTypeId(ColliderType::ColliderTypePlayer);

	exporter_.TryLoadFromJson();
}

void Boss::update() {
	//behavior->move(this);
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
	collider_->SetColliderPosition(transform_.GetWorldPosition());
}

void Boss::finalize() {
	exporter_.OutputToJson();
}

void Boss::SetAttributeImGui() {
}

void Boss::take_damage(int32_t damage) {
	hitpoint -= damage;
}

void Boss::set_model(std::string file) {
	ModelBehavior::model_ = SxavengerGame::LoadModel("Resources/model/CG2", file);
	model_->ApplyMeshShader();
}

bool Boss::is_dead() const {
	return hitpoint <= 0;
}

bool Boss::is_destroy() const {
	return isDestroy;
}
