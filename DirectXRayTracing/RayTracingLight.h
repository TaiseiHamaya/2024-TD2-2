#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// DxObject
#include <DxBufferResource.h>

// Geometry
#include <Vector4.h>
#include <Vector3.h>

// c++
#include <memory>

// attribute
#include <Attribute.h>

////////////////////////////////////////////////////////////////////////////////////////////
// RayTracingLight class
////////////////////////////////////////////////////////////////////////////////////////////
class RayTracingLight
	: public Attribute {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	RayTracingLight() { Init(); }

	~RayTracingLight() { Term(); }

	void Init();

	void Term();

	void SetAttributeImGui() override;

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const {
		return resource_->GetGPUVirtualAddress();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////
	// DirectionalLight structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct DirectionalLight {
		Vector4f color     = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3f direction = { 0.0f, 0.0f, 1.0f };
		float intensity    = 1.0f;
	};

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<DxObject::BufferPtrResource<DirectionalLight>> resource_;
	DirectionalLight data_;

};