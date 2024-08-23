#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* DxObjectCommon
#include <DxObjectCommon.h>

//* DirectX12
#include <dxcapi.h>

//* c++
#include <array>
#include <memory>

//* DxObject
#include <DxShaderBlob.h>
#include <DxShaderReflection.h>
#include <DxRootSignatureDesc.h>

//-----------------------------------------------------------------------------------------
// comment
//-----------------------------------------------------------------------------------------
#pragma comment(lib, "dxcompiler.lib")

////////////////////////////////////////////////////////////////////////////////////////////
// DxObject
////////////////////////////////////////////////////////////////////////////////////////////
_DXOBJECT_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////////////////
// GraphicsShaderNode class
////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsShaderNode
	: public BaseShaderBlob {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	GraphicsShaderNode() = default;
	~GraphicsShaderNode() = default;

	void Create(const std::wstring& filename, GraphicShaderType type);

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	//* node *//

	std::array<IDxcBlob*, GraphicShaderType::kCountOfGraphicShaderType> nodes_;

	bool isUseMeshPipeline_ = false;

	//=========================================================================================
	// private methods
	//=========================================================================================

};

_DXOBJECT_NAMESPACE_END