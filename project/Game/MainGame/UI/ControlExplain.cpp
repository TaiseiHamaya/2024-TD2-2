#include "ControlExplain.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>
#include <Engine/Game/SxavengerGame.h>

//* lib
#include <Lib/Easing.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ControlExplain class methods
////////////////////////////////////////////////////////////////////////////////////////////

void ControlExplain::Init() {
	SetName("control explain");

	ModelBehavior::model_         = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "control_explain.obj");
	ModelBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	material_.material.roughness = 1.0f;
	material_.Transfer();

	transform_.transform.scale = { 4.0f, 4.0f, 4.0f };
	transform_.UpdateMatrix();

	textures_[0] = Sxavenger::LoadTexture("resourcesData/GameScene/Model/control_explain1.png");
	textures_[1] = Sxavenger::LoadTexture("resourcesData/GameScene/Model/control_explain2.png");

	texturesKey_[0] = Sxavenger::LoadTexture("resourcesData/GameScene/Model/control_explain_key1.png");
	texturesKey_[1] = Sxavenger::LoadTexture("resourcesData/GameScene/Model/control_explain_key2.png");
}

void ControlExplain::Term() {
}

void ControlExplain::Update(const Vector3f& position, const Quaternion& quaternion) {

	if (displayTimer_.time == 0.0f) {
		if (openAndCloseTimer_.time > 0.0f) {
			openAndCloseTimer_.SubtractDeltaTime();
			openAndCloseTimer_.time = std::max(openAndCloseTimer_.time, 0.0f);
		}
	} else {
		if (openAndCloseTimer_ < openAndCloseTime_) {
			openAndCloseTimer_.AddDeltaTime();
			openAndCloseTimer_.time = std::min(openAndCloseTimer_.time, openAndCloseTime_.time);

		} else {
			displayTimer_.SubtractDeltaTime();
			displayTimer_.time = std::max(displayTimer_.time, 0.0f);
		}
	}

	float t = openAndCloseTimer_.time / openAndCloseTime_.time;
	transform_.transform.scale = Lerp(kOrigin3, { 4.0f, 4.0f, 4.0f }, EaseOutBack(t));

	transform_.transform.translate = position + offset_;
	transform_.transform.rotate    = quaternion;
	transform_.UpdateMatrix();
}

void ControlExplain::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	auto commandList = Sxavenger::GetCommandList();

	Texture* texture = texturesKey_[textureIndex_];

	auto input = Sxavenger::GetInput()->GetGamepadInput(0);
	if (input->IsConnect()) {
		texture = textures_[textureIndex_];
	}

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		if (model_->GetMesh(i).IsCreateMeshlet()) {

			sSystemConsole->SetRenderingPipeline(kDefaultMS_AlbedoPS_Deferred);
			//!< meshlet生成がされているのでmesh shaderで描画

			commandList->SetGraphicsRootConstantBufferView(5, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(7, transform_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(8, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(9, texture->GetGPUHandleSRV());
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
			commandList->SetGraphicsRootDescriptorTable(3, texture->GetGPUHandleSRV());
			commandList->SetGraphicsRootConstantBufferView(4, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(5, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall();
		}
	}
}

void ControlExplain::SetExplainParameter(DeltaTimePoint displayTime, uint32_t textureIndex) {
	displayTimer_ = displayTime;
	textureIndex_ = textureIndex;
}

bool ControlExplain::IsDisplayExplain() const {
	return !((displayTimer_.time == 0.0f) && (openAndCloseTimer_.time == 0.0f));
}
