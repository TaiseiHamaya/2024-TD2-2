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
	// コライダー設定
	SetColliderBoundingSphere();
}

void Boss::update() {
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
}

const Vector3f& Boss::GetColliderPosition() const {
	return worldPosition;
}
