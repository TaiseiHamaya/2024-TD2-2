#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/AnimationBehavior.h>

//* engine
#include <Engine/Game/SxavengerPipeline/VisualProcessLayer.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitlePlayer class
////////////////////////////////////////////////////////////////////////////////////////////
class TitlePlayer
	: public AnimationBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	TitlePlayer() = default;
	~TitlePlayer() = default;

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

};