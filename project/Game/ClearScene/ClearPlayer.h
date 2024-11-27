#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/AnimationBehavior.h>

//* engine
#include <Engine/Game/SxavengerPipeline/VisualProcessLayer.h>

//* game
#include <Game/MainGame/UI/ControlExplain.h>


class ClearPlayer
	: public AnimationBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	ClearPlayer() = default;
	~ClearPlayer() = default;

	void Init();

	void Term();

	void Update();

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<Animator> animator_;
	DeltaTimePoint animationTime_;

	std::unique_ptr<VisualProcessDoF> dof_;

	std::unique_ptr<ControlExplain> explain_;
	uint32_t explainIndex_ = 0;
};

