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
