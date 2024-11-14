#include "MainGameScene.h"

#include <Engine/Game/Camera/Camera3D.h>
#include <Engine/Console/SystemConsole.h>

#include <Lib/Geometry/MathLib.h>

void MainGameScene::Init() {
	groundTest = std::make_unique<TestGround>();
	groundTest->Init();
	groundTest->SetToConsole();

	player = std::make_unique<PlayerManager>();
	player->initialize();

	bossManager = std::make_unique<BossManager>();
	bossManager->initialize();

	SystemConsole::GetInstance()->GetGameCamera()->SetTransform(kUnit3, Vector3{ 45.0f * kDegToRad ,0.0f,0.0f }, Vector3{ 0,10,-10 });
}

void MainGameScene::Term() {
}

void MainGameScene::Update() {
	player->begin();
	player->update();
	bossManager->update();

	player->update_matrix();
	bossManager->update_matrix();
}

void MainGameScene::Draw() {
}
