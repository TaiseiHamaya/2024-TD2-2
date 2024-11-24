#include "TitleBoard.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitleBoard class
////////////////////////////////////////////////////////////////////////////////////////////

void TitleBoard::Init() {
	SetName("titleBorad");

	ModelBehavior::model_ = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "title.obj");

	ModelBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	ModelBehavior::TryLoadJson();

}

void TitleBoard::Term() {
}

void TitleBoard::Update() {
}
