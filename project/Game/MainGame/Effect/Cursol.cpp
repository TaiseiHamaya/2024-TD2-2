#include "Cursol.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>

//* lib
#include <Lib/Easing.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Cursol class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Cursol::Init() {
	SetName("cursol");

	AnimationBehavior::renderingFlag_ = kBehaviorRender_Systematic;
	AnimationBehavior::model_         = SxavengerGame::LoadModel("resourcesData/gameScene/model", "arrow.gltf");

	material_.material.metallic  = 0.75f;
	material_.material.roughness = 0.75f;
	material_.Transfer();

	animtor_ = std::make_unique<Animator>(model_);

	AnimationBehavior::animator_ = animtor_.get();
}

void Cursol::Term() {
}

void Cursol::Update(const Vector3f& position) {

	animationTime_.AddDeltaTime();

	for (uint32_t i = 0; i < animtor_->GetAnimationSize(); ++i) {
		animtor_->Update(animationTime_, i, true);
	}

	transform_.transform.translate = position + offset;
	//transform_.transform.rotate    = MakeAxisAngle({0.0f, 1.0f, 0.0}, std::lerp(0.0f, pi_v, t));
	//transform_.transform.scale     = { 0.8f, 0.8f, 0.8f };
}

void Cursol::UpdateMatrix() {
	transform_.UpdateMatrix();
}
