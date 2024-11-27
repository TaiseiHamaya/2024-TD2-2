#include "SceneManager.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>
#include <Engine/Game/SxavengerGame.h>

#include <Lib/Easing.h>

////////////////////////////////////////////////////////////////////////////////////////////
// SceneManager class methods
////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::Init(std::unique_ptr<IScene>& scene) {
	scene_ = std::move(scene);
	scene_->Init();
	CreateChanger();
}

void SceneManager::CreateChanger() {
	changeTexture = Sxavenger::LoadTexture("resources/white1x1.png");
}

void SceneManager::Term() {
}

void SceneManager::Update() {
	scene_->Update();

	if (nextScene_ == nullptr && scene_->IsRequsetedNextScene()) {
		nextScene_ = scene_->GetNextScene(); //!< シーンの変更
	}

	if (nextScene_ != nullptr) {
		//!< 次シーンへの遷移
		if (changeTimer_ < changeTime_) {
			changeTimer_.AddDeltaTime();
			changeTimer_.time = std::min(changeTimer_.time, changeTime_.time);

			if (changeTimer_ >= changeTime_) {
				isNextSceneRequest_ = true;
			}
		}
		

	} else {
		//!< 現シーンへの遷移
		if (changeTimer_.time > 0.0f) {
			changeTimer_.SubtractDeltaTime();
			changeTimer_.time = std::max(changeTimer_.time, 0.0f);
		}
	}
}

void SceneManager::Draw() {
	scene_->Draw();

	if (changeTimer_.time > 0.0f) {

		float t = changeTimer_.time / changeTime_.time;
		Vector2f size = Lerp({ 0.0f, 0.0f }, kWindowSize, EaseOutBack(t));

		sSystemConsole->GetGameFrame()->BeginAdaptive();

		auto drawer = SxavengerGame::GetSpriteCommon();

		drawer->DrawSprite(
			kWindowSize / 2u, size, { 0.5f, 0.5f }, changeTexture->GetGPUHandleSRV(), { 0.0f, 0.0f, 0.0f, 1.0f }
		);

		sSystemConsole->GetGameFrame()->EndAdaptive();
	}
}

void SceneManager::ChangeScene() {
	if (isNextSceneRequest_) {
		scene_ = std::move(nextScene_);
		scene_->Init();
		isNextSceneRequest_ = false;
	}
}