#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// directX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <cstdint>
#include <cassert>
#include <vector>

// ComPtr
#include <ComPtr.h>

// DxObject
#include <DxObjectMethod.h>

//-----------------------------------------------------------------------------------------
// comment
//-----------------------------------------------------------------------------------------
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

////////////////////////////////////////////////////////////////////////////////////////////
// DxObject namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace DxObject { //!< DxSource

	////////////////////////////////////////////////////////////////////////////////////////////
	// CSRootSignatureDesc class
	////////////////////////////////////////////////////////////////////////////////////////////
	class CSRootSignatureDesc {

		//=========================================================================================
		// public methods
		//=========================================================================================

		~CSRootSignatureDesc() { Clear(); }

		void Resize(uint32_t paramSize, uint32_t samplerSize);

		void Clear();

		void SetCBV(uint32_t index, UINT shaderRegister);

		void SetSRV(uint32_t index, UINT shaderRegister);

		void SetUAV(uint32_t index, UINT shaderRegister);

		void SetSampler(uint32_t index, TextureMode mode, UINT shaderRegister);

		//=========================================================================================
		// public variables
		//=========================================================================================

		std::vector<D3D12_ROOT_PARAMETER>      params;
		std::vector<D3D12_STATIC_SAMPLER_DESC> samplers;

	private:

		//=========================================================================================
		// private varibles
		//=========================================================================================

		std::vector<D3D12_DESCRIPTOR_RANGE> ranges;

	};

	////////////////////////////////////////////////////////////////////////////////////////////
	// CSStateObject class
	////////////////////////////////////////////////////////////////////////////////////////////
	class CSPipelineState {
	public:

		//=========================================================================================
		// public methods
		//=========================================================================================

		CSPipelineState() {}

		~CSPipelineState() { Term(); }

		void Init();

		void Term();



	private:

		//=========================================================================================
		// private varibles
		//=========================================================================================

		ComPtr<ID3D12RootSignature> rootSignature_;
		ComPtr<ID3D12PipelineState> pipelineState_;

	};

}