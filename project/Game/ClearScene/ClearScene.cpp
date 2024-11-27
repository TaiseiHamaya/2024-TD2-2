#include "ClearScene.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* other scene
#include <Game/TitleScene/TitleScene.h>

//* engine
#include <Engine/System/Sxavenger.h>
#include <Engine/Console/SystemConsole.h>

void ClearScene::Init() {
	sSystemConsole->GetGameCamera()->Reset();

	field_ = std::make_unique<Field>();
	field_->Init();
	field_->SetToConsole();

	clearPlayer = std::make_unique<ClearPlayer>();
	clearPlayer->Init();
	clearPlayer->SetToConsole();

	sSystemConsole->GetGameCamera()->SetTransform(kUnit3, MakeAxisAngle({1,0,0}, 0.4f), {0, 5, -10});
}

void ClearScene::Term() {
}

void ClearScene::Update() {
	field_->Update();
	clearPlayer->Update();

	auto input = Sxavenger::GetInput()->GetGamepadInput(0);
	if (input->IsTriggerButton(XINPUT_GAMEPAD_A)) {
		SetNextScene<TitleScene>();
	}
}

void ClearScene::Draw() {
}
