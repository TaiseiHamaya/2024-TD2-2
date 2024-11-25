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

	ModelBehavior::renderingFlag_ = kBehaviorRender_Systematic;
	ModelBehavior::model_         = SxavengerGame::LoadModel("resourcesData/gameScene/model", "player_cursol_demo.obj");

	material_.material.metallic  = 0.75f;
	material_.material.roughness = 0.75f;
	material_.Transfer();

}

void Cursol::Term() {
}

void Cursol::Update(const Vector3f& position) {

	roundTimer_.AddDeltaTime();
	roundTimer_.time = std::fmod(roundTimer_.time, roundTime_.time);

	float t = EaseOutExpo(roundTimer_.time / roundTime_.time);

	transform_.transform.translate = position + offset;
	transform_.transform.rotate    = MakeAxisAngle({0.0f, 1.0f, 0.0}, std::lerp(0.0f, pi_v, t));
	transform_.transform.scale     = { 0.8f, 0.8f, 0.8f };
}

void Cursol::UpdateMatrix() {
	transform_.UpdateMatrix();

}
