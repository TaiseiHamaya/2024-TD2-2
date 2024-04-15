#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// directX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <cstdint>

// ComPtr
#include <ComPtr.h>

//-----------------------------------------------------------------------------------------
// comment
//-----------------------------------------------------------------------------------------
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

////////////////////////////////////////////////////////////////////////////////////////////
// DxrObjectMethod namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace DxrObjectMethod {

	ComPtr<ID3D12Resource> CreateBufferResource(
		ID3D12Device5* device,
		uint64_t size
	);

}