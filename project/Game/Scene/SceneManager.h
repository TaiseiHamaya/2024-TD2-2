#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* scene
#include "IScene.h"

//* engine
#include <Engine/System/Performance.h>
#include <Engine/System/Texture.h>

////////////////////////////////////////////////////////////////////////////////////////////
// SceneManager class
////////////////////////////////////////////////////////////////////////////////////////////
class SceneManager {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! [template]
	//! スタート時のsceneの設定
	template <DerivedFromIScene T>
	void Init();

	void Init(std::unique_ptr<IScene>& scene);

	void CreateChanger();

	void Term();

	void Update();

	void Draw();

	void ChangeScene();

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	//* scene *//

	std::unique_ptr<IScene> scene_ = nullptr;
	std::unique_ptr<IScene> nextScene_ = nullptr;

	//* scene changer *//

	DeltaTimePoint changeTime_  = { 0.8f };
	DeltaTimePoint changeTimer_ = {};

	bool isNextSceneRequest_ = false;

	Texture* changeTexture = nullptr;

};

////////////////////////////////////////////////////////////////////////////////////////////
// SceneManager class template methods
////////////////////////////////////////////////////////////////////////////////////////////

template<DerivedFromIScene T>
inline void SceneManager::Init() {
	scene_ = std::make_unique<T>();
	scene_->Init();
	CreateChanger();
}
