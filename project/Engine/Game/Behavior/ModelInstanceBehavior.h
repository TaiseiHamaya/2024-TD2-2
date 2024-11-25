#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include "BaseBehavior.h"

//* engine
//#include <Engine/System/DxrObject/DxrBufferRecorder.h>
#include <Engine/Game/Model.h>
#include <Engine/Game/Transform.h>
#include <Engine/Game/Material.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ModelInstanceBehavior class
////////////////////////////////////////////////////////////////////////////////////////////
class ModelInstanceBehavior
	: public BaseBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	ModelInstanceBehavior() { Init(); }
	~ModelInstanceBehavior() { Term(); }

	void Init();

	void Term();

	void CreateInstance(uint32_t size);

	//* derivative behaivor methods *//

	virtual void SystemAttributeImGui() override;

	virtual void DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) override;
	virtual void DrawAdaptive(_MAYBE_UNUSED const Camera3D* camera) override;

	//* external *//

	void OutputJson();
	void TryLoadJson(const std::string& filename = "");

protected:

	//=========================================================================================
	// protected variables
	//=========================================================================================

	//* IA *//
	Model* model_ = nullptr;

	//* buffer *//
	std::unique_ptr<DxObject::BufferResource<TransformationMatrix>> matrix_;

	PBRMaterialBuffer material_;
	UVTransformBuffer uvTransform_;
	ColorBuffer color_;


};