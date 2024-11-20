#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

void Boss::initialize() {

	SetName("Boss");
	// モデル設定
	ModelBehavior::model_ = SxavengerGame::LoadModel("Resources/model/CG2", "sphere.obj");
	model_->ApplyMeshShader();
	transform_.UpdateMatrix();
	renderingFlag_ = kBehaviorRender_Systematic;

	TryLoadJson();

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();

	exporter_.TryLoadFromJson();
}

void Boss::update() {
	collider_->SetColliderPosition(worldPosition);

}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
}

void Boss::finalize() {
	exporter_.OutputToJson();
}

void Boss::SetAttributeImGui() {
	exporter_.DragFloat3("test", &test_.x, 0.02f);
}
