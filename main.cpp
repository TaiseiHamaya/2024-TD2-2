#include <MyEngine.h> // sxavenger engine
#include <Environment.h>

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// GameScene
#include <GameScene.h>

// c++
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//=========================================================================================
	// 初期化
	//=========================================================================================
	MyEngine::Initialize(kWindowWidth, kWindowHeight, kWindowTitle);

	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	
	gameScene->Run();

	gameScene.reset();

	MyEngine::Finalize();
	return 0;

}