#include "Skydome.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Skydome class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Skydome::Init() {
	ModelBehavior::renderingFlag_ = kBehaviorRender_Adaptive;
	ModelBehavior::model_ = SxavengerGame::LoadModel("resourcesData/gameScene/model", "skydome.obj");

	material_.material.roughness = 1.0f;
	material_.material.roughness = 1.0f;
	material_.Transfer();

	color_.color = ToColor4f({ 21, 32, 32, 255 });
	color_.Transfer();

	transform_.transform.scale = { 4.0f, 4.0f, 4.0f };
	transform_.UpdateMatrix();
}

void Skydome::Term() {
}


