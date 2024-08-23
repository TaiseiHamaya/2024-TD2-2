#include "DxShaderReflection.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <DxShaderBlobManager.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ShaderReflectionTable class methods
////////////////////////////////////////////////////////////////////////////////////////////

void DxObject::ShaderReflectionTable::Create(IDxcBlob* blob, ShaderVisibility visibility) {

	ComPtr<ID3D12ShaderReflection> reflection = CreateReflection(blob);

	Reflect(reflection.Get(), visibility);

}

void DxObject::ShaderReflectionTable::Marge(ShaderReflectionTable* other) {

	// other情報をthisにmarge
	for (const auto& it : other->table_) {
		MargeBufferInfo(it.first, it.second.info);
	}

}

void DxObject::ShaderReflectionTable::Bind(const std::string& bufferName, const BindBuffer& buffer) {

	auto it = table_.find(bufferName);
	assert(it != table_.end()); //!< bufferNameが見つからなかった

	it->second.buffer = buffer;
}

DxObject::BaseRootSignatureDesc DxObject::ShaderReflectionTable::CreateRootSignatureDesc() {
	
	GraphicsRootSignatureDesc result = {};
	result.Resize(static_cast<uint32_t>(table_.size()) - samplerCount_, samplerCount_);

	uint32_t samplerIndex = 0;
	uint32_t paramIndex   = 0;

	for (auto& it : table_) {

		// buffer情報の参照取得
		const auto& info = it.second.info;
		auto& buffer     = it.second.buffer;

		if (info.bufferType == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER) {
			// samplerの設定
			result.SetSampler(samplerIndex, MODE_WRAP, info.visibility,	info.registerNum);

			samplerIndex++;

		} else {

			// ここの時点でBufferが入ってる必要がある
			assert(buffer.has_value());

			// paramの設定
			switch (info.bufferType) {
				case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER: //!< CBV
					if (std::holds_alternative<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value())) {
						result.SetCBV(paramIndex, info.visibility, info.registerNum);

					} else if (std::holds_alternative<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value())) {
						assert(false); //!< handleでの設定は不可
					}
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE: //!< Texture dimension
					if (std::holds_alternative<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value())) {
						assert(false); //!< virtualでの設定は不可

					} else if (std::holds_alternative<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value())) {
						result.SetSRV(paramIndex, info.visibility, info.registerNum);
					}
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED: //!< SRV
					if (std::holds_alternative<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value())) {
						result.SetVirtualSRV(paramIndex, info.visibility, info.registerNum);

					} else if (std::holds_alternative<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value())) {
						result.SetSRV(paramIndex, info.visibility, info.registerNum);
					}
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED: //!< UAV
					if (std::holds_alternative<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value())) {
						result.SetVirtualUAV(paramIndex, info.visibility, info.registerNum);

					} else if (std::holds_alternative<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value())) {
						result.SetUAV(paramIndex, info.visibility, info.registerNum);
					}
					break;

				default:
					assert(false); //!< 例外のbufferType
					break;
			}

			// TODO: buffer情報にparameterIndexの情報を関連付け
			it.second.paramIndex = paramIndex; //!< commandListで使用するので保存

			paramIndex++;
		}
	}

	return result;
}

void DxObject::ShaderReflectionTable::BindGraphicsParameter(ID3D12GraphicsCommandList* commandList) {

	for (const auto& it : table_) {

		// buffer情報の参照取得
		const auto& info   = it.second.info;
		const auto& buffer = it.second.buffer;
		const auto& index  = it.second.paramIndex;

		if (info.bufferType == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER) { //!< samplerの設定はしない
			continue;
		}

		assert(buffer.has_value()); //!< bufferを持ってないといけない

		if (std::holds_alternative<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value())) {

			// vartualAddressの取得
			const D3D12_GPU_VIRTUAL_ADDRESS& address = std::get<D3D12_GPU_VIRTUAL_ADDRESS>(buffer.value());

			switch (info.bufferType) {
				case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER: //!< CBV
					commandList->SetGraphicsRootConstantBufferView(index.value(), address);
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED: //!< SRV
					commandList->SetGraphicsRootShaderResourceView(index.value(), address);
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED: //!< UAV
					commandList->SetGraphicsRootUnorderedAccessView(index.value(), address);
					break;

				default:
					assert(false); //!< 例外
					break;
			}

		} else if (std::holds_alternative<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value())) {

			// handleの取得
			const D3D12_GPU_DESCRIPTOR_HANDLE& handle = std::get<D3D12_GPU_DESCRIPTOR_HANDLE>(buffer.value());

			commandList->SetGraphicsRootDescriptorTable(index.value(), handle);

		}
	}

}

void DxObject::ShaderReflectionTable::MargeBufferInfo(const std::string& key, const BufferInfo& value) {

	auto it = table_.find(key);
	if (it == table_.end()) { //!< tableに登録されていない場合

		// 新しく登録
		table_.emplace(key, BufferTable{ value, /*bindBuffer*/});

		if (value.bufferType == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER) { //!< samplerの場合
			samplerCount_++;
		}

	} else { //!< 登録済み

		//* コンフリクトの確認
		//* ここに入った場合, visibilityがallにできるか確認
		// buffer種類の確認
		assert(value.bufferType != it->second.info.bufferType); //!< 同じ名前なのに違うbufferの種類である

		// register番号の確認
		assert(value.registerNum != it->second.info.registerNum); //!< register番号が一致しない

		// visibilityをallに変更
		it->second.info.visibility = VISIBILITY_ALL;
	}
}

ComPtr<ID3D12ShaderReflection> DxObject::ShaderReflectionTable::CreateReflection(IDxcBlob* blob) {
	
	// reflectionの生成
	ComPtr<ID3D12ShaderReflection> result;

	DxcBuffer buffer = {
		.Ptr  = blob->GetBufferPointer(),
		.Size = blob->GetBufferSize(),
		//!< encordingは設定しない
	};

	assert(manager_ != nullptr); //!< managerが設定されてない

	auto hr = manager_->GetUtils()->CreateReflection(
		&buffer,
		IID_PPV_ARGS(&result)
	);

	assert(SUCCEEDED(hr));

	return result;
}

void DxObject::ShaderReflectionTable::Reflect(ID3D12ShaderReflection* reflection, ShaderVisibility visibility) {

	D3D12_SHADER_DESC mainDesc = {}; //!< shaderの外見情報
	reflection->GetDesc(&mainDesc);

	// 外部からBindされている情報の取得
	for (UINT i = 0; i < mainDesc.BoundResources; ++i) {

		// bind情報の取得
		D3D12_SHADER_INPUT_BIND_DESC bindDesc = {};
		reflection->GetResourceBindingDesc(i, &bindDesc);

		// bind情報をtableに保存
		MargeBufferInfo(bindDesc.Name, { bindDesc.Type, bindDesc.BindPoint, visibility });
	}

}