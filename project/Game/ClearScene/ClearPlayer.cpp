#include "ClearPlayer.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>

void ClearPlayer::Init() {
	SetName("ClearPlayer");

	AnimationBehavior::model_ = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "enemy_clear.gltf");

	animator_ = std::make_unique<Animator>(model_);
	AnimationBehavior::animator_ = animator_.get();

	AnimationBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	AnimationBehavior::TryLoadJson();

	transform_.transform.rotate = MakeAxisAngle({ 0,1,0 }, -2.7f);
	transform_.transform.translate.x = 3;
	transform_.UpdateMatrix();

	material_.material.metallic = 0.2f;
	material_.material.roughness = 0.8f;
	material_.Transfer();

	uvTransform_.transform.scale.x = 1.0f / 3.0f;
	uvTransform_.Transfer();
}

void ClearPlayer::Term() {
}

void ClearPlayer::Update() {

	animationTime_.AddDeltaTime();

	for (uint32_t i = 0; i < animator_->GetAnimationSize(); ++i) {
		animator_->Update(animationTime_, i);
	}
}
