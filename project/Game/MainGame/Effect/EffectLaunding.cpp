#include "EffectLaunding.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// EffectLaunding class methods
////////////////////////////////////////////////////////////////////////////////////////////

void EffectLaunding::Init() {
	SetName("EffectLaunding");
	CreateInstance(kInstanceCount_);

	for (uint32_t i = 0; i < kInstanceCount_; ++i) {
		(*matrix_)[i].Transfer(Matrix::MakeAffine(kOrigin3, kOrigin3, kOrigin3));
	}

	ModelInstanceBehavior::model_ = SxavengerGame::LoadModel("resourcesData/gameScene/Model", "dust.obj");
	ModelInstanceBehavior::TryLoadJson();

	ModelInstanceBehavior::renderingFlag_ = kBehaviorRender_Systematic;
}

void EffectLaunding::Term() {

}

void EffectLaunding::Update() {

	elements_.remove_if([](Element& element) {
		// timeの加算
		element.aliveTimer.AddDeltaTime();
		return element.aliveTimer >= element.aliveTime;
	});

	for (auto& element : elements_) {
		// transformの変更
		element.transform.translate += element.velocity * Performance::GetDeltaTime(s).time;
		element.transform.scale      = Lerp(kUnit3, kOrigin3, element.aliveTimer.time / element.aliveTime.time);
	}

	// 描画設定
	instanceIndex_ = 0;
	for (auto& element : elements_) {
		if (instanceIndex_ >= kInstanceCount_) {
			break;
		}

		(*matrix_)[instanceIndex_].Transfer(element.transform.ToMatrix());
		instanceIndex_++;
	}

}

void EffectLaunding::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	Assert(matrix_ != nullptr, "matrix instance buffer is not create.");

	auto commandList = Sxavenger::GetCommandList();

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		if (model_->GetMesh(i).IsCreateMeshlet()) {

			sSystemConsole->SetRenderingPipeline(kDefaultMS_AlbedoPS_Deferred);
			//!< meshlet生成がされているのでmesh shaderで描画

			commandList->SetGraphicsRootConstantBufferView(5, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(7, matrix_->GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(8, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(9, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(10, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(11, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).Dispatch(0, 1, 2, 3, 4, 6, instanceIndex_);

		} else {
			
			sSystemConsole->SetRenderingPipeline(kDefaultVS_AlbedoPS_Deferred);
			//!< vertex shaderで描画

			model_->GetMesh(i).BindIABuffer();

			commandList->SetGraphicsRootConstantBufferView(0, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(1, matrix_->GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(2, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(3, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(4, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(5, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall(instanceIndex_);
		}
	}
}
