#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// object
#include <RayTracingObject.h>

// model
#include <Model.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Player class
////////////////////////////////////////////////////////////////////////////////////////////
class Player
	: public RayTracingObject
	, public Attribute {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	Player() { Init(); }

	~Player() { Term(); }

	void Init();

	void Term();

	void Update();

	void SetAttributeImGui() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<Model> model_;

};