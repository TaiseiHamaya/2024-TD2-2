#include "TitleEnemy.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitleEnemy class
////////////////////////////////////////////////////////////////////////////////////////////

void TitleEnemy::Init() {
	SetName("titleEnemy");

	AnimationBehavior::model_ = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "enemy_title.gltf");

	animator_                    = std::make_unique<Animator>(model_);
	AnimationBehavior::animator_ = animator_.get();
	
	AnimationBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	AnimationBehavior::TryLoadJson();

	uvTransform_.transform.scale.x = 1.0f / 3.0f;
	uvTransform_.Transfer();
}

void TitleEnemy::Term() {
}

void TitleEnemy::Update() {
	animationTime_.AddDeltaTime();

	for (uint32_t i = 0; i < animator_->GetAnimationSize(); ++i) {
		animator_->Update(animationTime_, i);
	}
}
