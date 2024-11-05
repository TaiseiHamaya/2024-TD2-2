#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/ModelBehavior.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Sponza class
////////////////////////////////////////////////////////////////////////////////////////////
class Sponza
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	void Init();

	void SetAttributeImGui() override;


private:
};