#include "ModelBehavior.h"
_DXROBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>
#include <Lib/Adapter/Json/Json.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ModelBehavior class methods
////////////////////////////////////////////////////////////////////////////////////////////

void ModelBehavior::Init() {
	transform_.Create();
	uvTransform_.Create();
	material_.Create();
	color_.Create();
}

void ModelBehavior::Term() {
}

void ModelBehavior::CreateRaytracingRecorder() {
	recorders_.resize(model_->GetMeshSize());

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		recorders_[i] = std::make_unique<BufferRecoreder>();
		recorders_[i]->Create(sSystemConsole->GetRaytracingPipeline()->GetExport(kHitgroup_Behavior, 0));

		recorders_[i]->SetAddress(0, model_->GetMesh(i).GetVertexBuffer()->GetGPUVirtualAddress()); //!< Vertices
		recorders_[i]->SetAddress(1, model_->GetMesh(i).GetIndexBuffer()->GetGPUVirtualAddress());  //!< Indices
		recorders_[i]->SetAddress(2, uvTransform_.GetVirtualAddress());                             //!< UVTransform
		recorders_[i]->SetHandle(3, model_->GetTextureHandle(i));                                   //!< Albedo
		recorders_[i]->SetAddress(4, material_.GetGPUVirtualAddress());                             //!< PBRMaterial
	}
}

void ModelBehavior::SystemAttributeImGui() {
	if (ImGui::TreeNode("transform")) {
		transform_.SetImGuiCommand();
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

void ModelBehavior::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
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
			commandList->SetGraphicsRootDescriptorTable(9, model_->GetTextureHandle(i));
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
			commandList->SetGraphicsRootDescriptorTable(3, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(4, material_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(5, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall();
		}
	}
}

void ModelBehavior::DrawAdaptive(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	auto commandList = Sxavenger::GetCommandList();

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		if (model_->GetMesh(i).IsCreateMeshlet()) {

			sSystemConsole->SetRenderingPipeline(kDefaultMS_AlbedoPS);
			//!< meshlet生成がされているのでmesh shaderで描画

			commandList->SetGraphicsRootConstantBufferView(5, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(7, transform_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(8, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(9, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(10, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).Dispatch(0, 1, 2, 3, 4, 6);

		} else {
			
			sSystemConsole->SetRenderingPipeline(kDefaultVS_AlbedoPS);
			//!< vertex shaderで描画

			model_->GetMesh(i).BindIABuffer();

			commandList->SetGraphicsRootConstantBufferView(0, camera->GetGPUVirtualAddress());
			commandList->SetGraphicsRootShaderResourceView(1, transform_.GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(2, uvTransform_.GetVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(3, model_->GetTextureHandle(i));
			commandList->SetGraphicsRootConstantBufferView(4, color_.GetGPUVirtualAddress());

			model_->GetMesh(i).DrawCall();
		}
	}
}

void ModelBehavior::DrawRaytracing(_MAYBE_UNUSED DxrObject::TopLevelAS* tlas) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	Matrix4x4 mat = transform_.GetWorldMatrix();

	Assert(model_->GetMeshSize() <= recorders_.size(), "raytracing recorder not created.");

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		tlas->SetInstance(model_->GetMesh(i).GetBLAS(), mat, recorders_.at(i).get(), 0);
	}
}

void ModelBehavior::OutputJson() {

	Json root = Json::object();

	auto& transform = root["Transform"] = Json::object();
	transform["scale"]      = JsonAdapter::ToJson(transform_.transform.scale);
	transform["rotate"]     = JsonAdapter::ToJson(transform_.transform.rotate);
	transform["translate"]  = JsonAdapter::ToJson(transform_.transform.translate);

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

void ModelBehavior::TryLoadJson(const std::string& filename) {

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

	const auto& transform = data["Transform"];
	transform_.transform.scale     = JsonAdapter::ToVector3f(transform["scale"]);
	transform_.transform.rotate    = JsonAdapter::ToQuaternion(transform["rotate"]);
	transform_.transform.translate = JsonAdapter::ToVector3f(transform["translate"]);
	transform_.UpdateMatrix();

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
