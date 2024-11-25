#include "ModelInstanceBehavior.h"
_DXOBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>
#include <Lib/Adapter/Json/Json.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ModelInstanceBehavior class methods
////////////////////////////////////////////////////////////////////////////////////////////

void ModelInstanceBehavior::Init() {
	material_.Create();
	uvTransform_.Create();
	color_.Create();
}

void ModelInstanceBehavior::Term() {
}

void ModelInstanceBehavior::CreateInstance(uint32_t size) {
	matrix_ = std::make_unique<BufferResource<TransformationMatrix>>(Sxavenger::GetDevicesObj(), size);
}

void ModelInstanceBehavior::SystemAttributeImGui() {
	if (ImGui::TreeNode("instance")) {
		ImGui::Text("count: %d", matrix_->GetIndexSize());
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("material")) {
		material_.SetImGuiCommand();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("UV transform")) {
		uvTransform_.SetImGuiCommand();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("color")) {
		color_.SetImGuiCommand();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("json")) {
		if (ImGui::Button("output json")) {
			OutputJson();
		}
		ImGui::TreePop();
	}
}

void ModelInstanceBehavior::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
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

			model_->GetMesh(i).Dispatch(0, 1, 2, 3, 4, 6, matrix_->GetIndexSize());

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

			model_->GetMesh(i).DrawCall(matrix_->GetIndexSize());
		}
	}
}

void ModelInstanceBehavior::DrawAdaptive(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	Assert(matrix_ != nullptr, "matrix instance buffer is not create.");

	auto commandList = Sxavenger::GetCommandList();

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		if (model_->GetMesh(i).IsCreateMeshlet()) {

			sSystemConsole->SetRenderingPipeline(kDefaultMS_AlbedoPS);
			//!< meshlet生成がされているのでmesh shaderで描画

			commandList->SetGraphicsRootConstantBufferView(5, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(7, matrix_->GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(8, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(9, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(10, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).Dispatch(0, 1, 2, 3, 4, 6, matrix_->GetIndexSize());

		} else {
			
			sSystemConsole->SetRenderingPipeline(kDefaultVS_AlbedoPS);
			//!< vertex shaderで描画

			model_->GetMesh(i).BindIABuffer();

			commandList->SetGraphicsRootConstantBufferView(0, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(1, matrix_->GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(2, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(3, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(4, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall(matrix_->GetIndexSize());
		}
	}
}

void ModelInstanceBehavior::OutputJson() {

	Json root = Json::object();

	auto& uvTransform = root["UVTransform"] = Json::object();
	uvTransform["scale"]     = JsonAdapter::ToJson(uvTransform_.transform.scale);
	uvTransform["rotate"]    = uvTransform_.transform.rotate;
	uvTransform["translate"] = JsonAdapter::ToJson(uvTransform_.transform.translate);

	auto& material = root["material"] = Json::object();
	material["roughness"] = material_.material.roughness;
	material["metallic"]  = material_.material.metallic;

	auto& color = root["color"] = Json::object();
	color = JsonAdapter::ToJson(color_.color);

	JsonAdapter::WriteJson(kBehaviorDirectory + name_ + ".json", root);
}

void ModelInstanceBehavior::TryLoadJson(const std::string& filename) {

	std::string filepath = kBehaviorDirectory;

	if (filename.empty()) {
		filepath += name_ + ".json";

	} else {
		filepath += filename;
	}

	Json data;

	if (!JsonAdapter::TryLoadJson(filepath, data)) {
		return; //!< 読み込み失敗したら抜ける
	}

	const auto& uvTransform = data["UVTransform"];
	uvTransform_.transform.scale     = JsonAdapter::ToVector2f(uvTransform["scale"]);
	uvTransform_.transform.rotate    = uvTransform["rotate"];
	uvTransform_.transform.translate = JsonAdapter::ToVector2f(uvTransform["translate"]);
	uvTransform_.Transfer();

	const auto& material = data["material"];
	material_.material.roughness = material["roughness"];
	material_.material.metallic  = material["metallic"];
	material_.Transfer();

	const auto& color = data["color"];
	color_.color = JsonAdapter::ToColor4f(color);
	color_.Transfer();
}
