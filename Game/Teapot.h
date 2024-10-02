#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Beta/ModelBehavior.h>

//* c++
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////
// Teapot class
////////////////////////////////////////////////////////////////////////////////////////////
class Teapot
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	void Init();

	void SetAttributeImGui() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<Model> model_;
	EulerTransformBuffer transform_;

};