#include "Boss.h"

#include <Engine/Game/SxavengerGame.h>
#include <Engine/System/Sxavenger.h>

#include <Engine/System/Performance.h>

void Boss::initialize() {

	SetName("Boss");
	// モデル設定
	AnimationBehavior::model_ = SxavengerGame::LoadModel("ResourcesData/GameScene/Model", "enemy_stay.gltf");
	//model_->ApplyMeshShader();

	CreateAnimation();

	transform_.UpdateMatrix();
	renderingFlag_ = kBehaviorRender_Systematic;

	// コライダー設定
	collider_ = std::make_unique<Collider>();
	collider_->SetColliderBoundingSphere();
}

void Boss::update() {
	collider_->SetColliderPosition(worldPosition);

	AnimationBehavior::animationTime_.AddDeltaTime();
	/*UpdateAnimator(2, true);
	UpdateAnimator(1, true);*/
	UpdateAnimator(animationIndex_, true);
}

void Boss::update_matrix() {
	transform_.UpdateMatrix();
}

void Boss::SetAttributeImGui() {
	uint32_t min = 0;
	uint32_t max = animator_->GetAnimationSize() - 1;
	ImGui::SliderScalar("animation index", ImGuiDataType_U32, &animationIndex_, &min, &max);
}
