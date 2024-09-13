#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* sxavenger
#include <Sxavenger.h>
#include <SxavengerGraphics.h>

//* c++
#include <list>
#include <string>
#include <format>

////////////////////////////////////////////////////////////////////////////////////////////
// SxavengerProcess class
////////////////////////////////////////////////////////////////////////////////////////////
class SxavengerProcess { //!< test class
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	void Init();

	void Update();

	//* singleton *//

	static SxavengerProcess* GetInstance();
	SxavengerProcess()  = default;
	~SxavengerProcess() = default;

private:

	//=========================================================================================
	// private varaibles
	//=========================================================================================

	std::list<int> layers_;

};