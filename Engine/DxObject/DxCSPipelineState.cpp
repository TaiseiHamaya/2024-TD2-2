#include "DxCSPipelineState.h"

////////////////////////////////////////////////////////////////////////////////////////////
// CSRootSignatureDesc class methods
////////////////////////////////////////////////////////////////////////////////////////////

void DxObject::CSRootSignatureDesc::Resize(uint32_t paramSize, uint32_t samplerSize) {
	params.resize(paramSize);
	ranges.resize(paramSize);
	samplers.resize(samplerSize);
}

void DxObject::CSRootSignatureDesc::Clear() {
	params.clear();
	params.shrink_to_fit();
	samplers.clear();
	samplers.shrink_to_fit();
	ranges.clear();
	ranges.shrink_to_fit();
}

void DxObject::CSRootSignatureDesc::SetCBV(uint32_t index, UINT shaderRegister) {
	params[index].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[index].ShaderVisibility          = D3D12_SHADER_VISIBILITY_ALL;
	params[index].Descriptor.ShaderRegister = shaderRegister;
}

void DxObject::CSRootSignatureDesc::SetSRV(uint32_t index, UINT shaderRegister) {
	ranges[index].BaseShaderRegister                = shaderRegister;
	ranges[index].NumDescriptors                    = 1;
	ranges[index].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	ranges[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	params[index].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	params[index].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
	params[index].DescriptorTable.pDescriptorRanges   = &ranges[index];
	params[index].DescriptorTable.NumDescriptorRanges = 1;
}

void DxObject::CSRootSignatureDesc::SetUAV(uint32_t index, UINT shaderRegister) {
	ranges[index].BaseShaderRegister                = shaderRegister;
	ranges[index].NumDescriptors                    = 1;
	ranges[index].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	ranges[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	params[index].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	params[index].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
	params[index].DescriptorTable.pDescriptorRanges   = &ranges[index];
	params[index].DescriptorTable.NumDescriptorRanges = 1;
}

void DxObject::CSRootSignatureDesc::SetSampler(uint32_t index, TextureMode mode, UINT shaderRegister) {
	samplers[index].Filter           = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplers[index].AddressU         = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(mode);
	samplers[index].AddressV         = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(mode);
	samplers[index].AddressW         = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(mode);
	samplers[index].ComparisonFunc   = D3D12_COMPARISON_FUNC_NEVER;
	samplers[index].MaxLOD           = D3D12_FLOAT32_MAX;
	samplers[index].ShaderRegister   = shaderRegister;
	samplers[index].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
}

////////////////////////////////////////////////////////////////////////////////////////////
// CSStateObject class methods
////////////////////////////////////////////////////////////////////////////////////////////

void DxObject::CSPipelineState::Init() {
}

void DxObject::CSPipelineState::Term() {
}
