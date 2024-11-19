#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

void Boss::initialize() {

	SetName("Boss");
	// モデル設定
	ModelBehavior::model_ = SxavengerGame::LoadModel("ResourcesData/GameScene/model", "enemy_stay.gltf");
	//model_->ApplyMeshShader();
	transform_.UpdateMatrix();
	renderingFlag_ = kBehaviorRender_Systematic;

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();

	animator_ = std::make_unique<Animator>(model_);
	AnimationBehavior::animator_ = animator_.get();
}

void Boss::update() {
	collider_->SetColliderPosition(worldPosition);

	animationTime_.AddDeltaTime();
	animator_->Update(animationTime_, 0, true);
	animator_->Update(animationTime_, 1, true);
	animator_->Update(animationTime_, 2, true);
	animator_->Update(animationTime_, 3, true);
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
}
