#pragma once
#define NOMINMAX

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Beta/BaseBehavior.h>
#include <Engine/Beta/ModelBehavior.h>

//* engine
#include <Engine/Beta/CineCamera.h>

//* Game
#include "Rail.h"


////////////////////////////////////////////////////////////////////////////////////////////
// Rail class
////////////////////////////////////////////////////////////////////////////////////////////
class RailCamera
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	RailCamera()  = default;
	~RailCamera() = default;

	void Init();

	void Term();

	void Update();

	void Draw();

	void SetAttributeImGui() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	//* external *//

	CineCamera* camera_ = nullptr;

	//* member *//

	std::unique_ptr<Rail> rail_;

	Quaternion rotate_ = Quaternion::Identity();

	Vector3f upVector_;

	float t_ = 0.0f;

	//=========================================================================================
	// private methods
	//=========================================================================================

	const Vector3f CalculateUpVector();


};