#include "MainGameScene.h"

#include <Engine/Game/Camera/Camera3D.h>
#include <Engine/Console/SystemConsole.h>
#include <Engine/Game/SxavengerGame.h>

#include <Lib/Geometry/MathLib.h>

void MainGameScene::Init() {
	groundTest = std::make_unique<TestGround>();
	groundTest->Init();
	groundTest->SetToConsole();

	playerManager = std::make_unique<PlayerManager>();
	playerManager->initialize();

	bossManager = std::make_unique<BossManager>();
	bossManager->initialize();

	field_ = std::make_unique<Field>();
	field_->Init();
	field_->SetToConsole();

	SystemConsole::GetInstance()->GetGameCamera()->SetTransform(kUnit3, Vector3{ 45.0f * kDegToRad ,0.0f,0.0f }, Vector3{ 0,10,-10 });
}

void MainGameScene::Term() {
}

void MainGameScene::Update() {
	playerManager->begin();
	playerManager->update();
	bossManager->update();

	playerManager->update_matrix();
	bossManager->update_matrix();

	SxavengerGame::UpdateColliders();

	playerManager->marge_collision();

	field_->Update();
}

void MainGameScene::Draw() {
}
