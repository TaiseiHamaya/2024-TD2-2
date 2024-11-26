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
	ModelBehavior::model_         = SxavengerGame::LoadModel("resourcesData/gameScene/model", "player_cursor.obj");

	material_.material.metallic  = 0.75f;
	material_.material.roughness = 0.75f;
	material_.Transfer();

	transform_.transform.scale = { 2.0f, 2.0f, 2.0f };
	transform_.transform.translate.y = 0.02f;
	transform_.UpdateMatrix();

}

void Cursol::Term() {
}

void Cursol::Update(const Vector3f& position) {

	transform_.transform.translate.x = position.x;
	transform_.transform.translate.z = position.z;

	//transform_.transform.rotate    = MakeAxisAngle({0.0f, 1.0f, 0.0}, std::lerp(0.0f, pi_v, t));
	//
}

void Cursol::UpdateMatrix() {
	transform_.UpdateMatrix();
}
