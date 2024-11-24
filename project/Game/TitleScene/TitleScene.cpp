#include "TitleScene.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* other scene
#include <Game/MainGame/MainGameScene.h>

//* engine
#include <Engine/System/Sxavenger.h>
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitleScene class methods
////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Init() {
	sSystemConsole->GetGameCamera()->Reset();

	field_ = std::make_unique<Field>();
	field_->Init();
	field_->SetToConsole();

	enemy_ = std::make_unique<TitleEnemy>();
	enemy_->Init();
	enemy_->SetToConsole();

	player_ = std::make_unique<TitlePlayer>();
	player_->Init();
	player_->SetToConsole();

	board_ = std::make_unique<TitleBoard>();
	board_->Init();
	board_->SetToConsole();

	sSystemConsole->GetGameCamera()->SetTransform(kUnit3, MakeAxisAngle({ 1.0f, 0.0f, 0.0f }, pi_v / 32.0f), {0.0f, 4.0f, -28.0f});
}

void TitleScene::Term() {
}

void TitleScene::Update() {

	field_->Update();
	enemy_->Update();
	player_->Update();

	if (Sxavenger::IsTriggerKey(DIK_SPACE)) {
		SetNextScene<MainGameScene>();
	}

}

void TitleScene::Draw() {
}
