#include "Shadow.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Shadow class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Shadow::Init() {
	SetName("shadow");
	ModelBehavior::model_         = SxavengerGame::LoadModel("resourcesData/gameScene/model", "player_cursor.obj");
	ModelBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	transform_.transform.translate.y = 0.06f;
	transform_.UpdateMatrix();

	material_.material.roughness = 1.0f;
	material_.Transfer();

	texture_ = Sxavenger::LoadTexture("resourcesData/gameScene/model/shadow.png");
}

void Shadow::Term() {
}

void Shadow::Update(float baseHeight, const Vector3f& position) {
	float length = position.y - baseHeight;

	float t = std::clamp(length, 0.0f, 10.0f) / 10.0f;

	transform_.transform.scale = Lerp({3.0f, 3.0f, 3.0f}, { 2.0f, 2.0f, 2.0f }, t);

	transform_.transform.translate.x = position.x;
	transform_.transform.translate.z = position.z;
	transform_.UpdateMatrix();
}

void Shadow::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	auto commandList = Sxavenger::GetCommandList();

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		if (model_->GetMesh(i).IsCreateMeshlet()) {

			sSystemConsole->SetRenderingPipeline(kDefaultMS_AlbedoPS_Deferred);
			//!< meshlet生成がされているのでmesh shaderで描画

			commandList->SetGraphicsRootConstantBufferView(5, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(7, transform_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(8, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(9, texture_->GetGPUHandleSRV());
			commandList->SetGraphicsRootConstantBufferView(10, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(11, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).Dispatch(0, 1, 2, 3, 4, 6);

		} else {
			
			sSystemConsole->SetRenderingPipeline(kDefaultVS_AlbedoPS_Deferred);
			//!< vertex shaderで描画

			model_->GetMesh(i).BindIABuffer();

			commandList->SetGraphicsRootConstantBufferView(0, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(1, transform_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(2, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(3, texture_->GetGPUHandleSRV());
			commandList->SetGraphicsRootConstantBufferView(4, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(5, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall();
		}
	}
}
