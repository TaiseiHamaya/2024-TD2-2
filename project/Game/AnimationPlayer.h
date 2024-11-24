#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/AnimationBehavior.h>

//* engine
#include <Engine/Beta/Curve.h>
#include <Engine/Game/Collider/Collider.h>

////////////////////////////////////////////////////////////////////////////////////////////
// AnimationPlayer class
////////////////////////////////////////////////////////////////////////////////////////////
class AnimationPlayer
	: public AnimationBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	AnimationPlayer()  = default;
	~AnimationPlayer() = default;

	void Init();

	void Term();

	void Update();

	void SetAttributeImGui() override;

private:

	//=========================================================================================
	// private varibales
	//=========================================================================================

	Curve test_curve;

	std::unique_ptr<Collider> testA_;
	std::unique_ptr<Collider> testB_;

	Vector3f positionA = {};
	Vector3f positionB = {};

	std::unique_ptr<Animator> animator_;
	DeltaTimePoint            animationTime_;

};