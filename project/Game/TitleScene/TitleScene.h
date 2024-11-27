#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Game/Scene/IScene.h>

//* Game
#include <Game/MainGame/Field.h>
#include <Game/TitleScene/TitleEnemy.h>
#include <Game/TitleScene/TitlePlayer.h>
#include <Game/TitleScene/TitleBoard.h>
#include <Game/MainGame/Skydome.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitleScene class
////////////////////////////////////////////////////////////////////////////////////////////
class TitleScene
	: public IScene {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	void Init() override;

	void Term() override;

	void Update() override;

	void Draw() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<Field> field_;
	std::unique_ptr<TitleEnemy> enemy_;
	std::unique_ptr<TitlePlayer> player_;
	std::unique_ptr<TitleBoard> board_;
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Audio> bgm_;

};