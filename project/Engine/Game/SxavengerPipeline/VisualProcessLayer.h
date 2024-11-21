#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/System/DxObject/DxBufferResource.h>
#include <Engine/Game/SxavengerPipeline/SxavengerFrame.h>
#include <Engine/Game/Camera/Camera3D.h>

//* lib
#include <Lib/CXXAttributeConfig.h>

//* c++
#include <string>
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////
// VisualProcessLayer class
////////////////////////////////////////////////////////////////////////////////////////////
class BaseVisualProcessLayer {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	BaseVisualProcessLayer()          = default;
	virtual ~BaseVisualProcessLayer() { Term(); }

	void Term();

	//* interface *//

	virtual void Process(_MAYBE_UNUSED SxavengerFrame* frame) {}

	virtual void SetLayerImGui() {}

	//* cosole option *//

	void SetToConsole();
	void SetToConsole(const std::string& name);

	//* getter *//

	const std::string& GetName() const { return name_; }

	//* setter *//

	void SetName(const std::string& name) { name_ = name; }

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::string name_ = "new base layer";

};

////////////////////////////////////////////////////////////////////////////////////////////
// VisualProcessDoF class
////////////////////////////////////////////////////////////////////////////////////////////
class VisualProcessDoF //!< 被写界深度. Depth of Field.
	: public BaseVisualProcessLayer {
public:

	////////////////////////////////////////////////////////////////////////////////////////////
	// Parameter structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct Parameter {
		float focusLength;
		float f;
		uint32_t isDebugDoF;
	};

public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	VisualProcessDoF()  = default;
	~VisualProcessDoF() = default;

	void Init();

	void Term();

	//* interface *//

	void Process(_MAYBE_UNUSED SxavengerFrame* frame) override;

	void SetLayerImGui() override;

	//* dof option *//

	void SetForcus(const Camera3D* camera, const Vector3f& position);

	//* getter *//

	const Parameter& GetParameter() const { return (*parameter_)[0]; }
	Parameter& GetParameter() { return (*parameter_)[0]; }

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<DxObject::BufferResource<Parameter>> parameter_;

};