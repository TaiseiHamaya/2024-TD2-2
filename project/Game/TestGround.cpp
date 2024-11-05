#include "TestGround.h"

#include <Engine/Game/SxavengerGame.h>

void TestGround::Init() {
	SetName("TestGround");

	ModelBehavior::model_ = SxavengerGame::LoadModel("Resources/model/TestModel", "Grid.obj");
	model_->ApplyMeshShader();

	transform_.transform.scale = { 1.0f, 1.0f, 1.0f };
	transform_.UpdateMatrix();

	renderingFlag_ = kBehaviorRender_Systematic;
}
