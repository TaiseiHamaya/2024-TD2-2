#include "Field.h"
_DXOBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// FieldTree class methods
////////////////////////////////////////////////////////////////////////////////////////////

void FieldTree::Init(uint32_t id, DeltaTimePoint startTime) {
	AnimationBehavior::model_ = SxavengerGame::LoadModel("resourcesData/gameScene/model", "tree.gltf");
	AnimationBehavior::renderingFlag_ = kBehaviorRender_Systematic;
	SetName("tree" + std::to_string(id));

	animator_ = std::make_unique<Animator>(model_);
	AnimationBehavior::animator_ = animator_.get();
	AnimationBehavior::TryLoadJson();

	animationTimer_ = startTime;

	exporter_.TryLoadFromJson();

	matrix_ = std::make_unique<BufferResource<TransformationMatrix>>(Sxavenger::GetDevicesObj(), kInstanceCount_);

	for (uint32_t i = 0; i < kInstanceCount_; ++i) {
		scales_[i] = { kUnit3 };

		// stashからの取り出し
		exporter_.GetFromStash(std::string("position" + std::to_string(i)).c_str(), &positions_[i].x, 3);
		exporter_.GetFromStash(std::string("scale" + std::to_string(i)).c_str(), &scales_[i].x, 3);

		// matrixに設定
		(*matrix_)[i].Transfer(Matrix::MakeAffine(scales_[i], {0.0f, pi_v / 2.0f, 0.0f}, positions_[i]));
	}
}

void FieldTree::Term() {
}

void FieldTree::Update() {
	animationTimer_.AddDeltaTime();
	animationTimer_.time = std::fmod(animationTimer_.time, animator_->GetAnimationDuration(0).time);

	animator_->Update(animationTimer_, 0, true);
}

void FieldTree::DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) {
	if (model_ == nullptr) {
		return; //!< modelが設定されていない
	}

	//!< animatorの描画

	auto commandList = Sxavenger::GetCommandList();

	// TODO: mesh shaderへの対応
	sSystemConsole->SetRenderingPipeline(kDefaultVS_AlbedoPS_Deferred);

	for (uint32_t i = 0; i < model_->GetMeshSize(); ++i) {
		// animator適用のIAを設定
		animator_->BindIABuffer(i);

		commandList->SetGraphicsRootConstantBufferView(0, camera->GetGPUVirtualAddress());
		commandList->SetGraphicsRootShaderResourceView(1, matrix_->GetGPUVirtualAddress());
		commandList->SetGraphicsRootConstantBufferView(2, uvTransform_.GetVirtualAddress());
		commandList->SetGraphicsRootDescriptorTable(3, model_->GetTextureHandle(i));
		commandList->SetGraphicsRootConstantBufferView(4, material_.GetGPUVirtualAddress());
		commandList->SetGraphicsRootConstantBufferView(5, color_.GetGPUVirtualAddress());

		model_->GetMesh(i).DrawCall(kInstanceCount_);
	}
}

void FieldTree::SetAttributeImGui() {
	for (uint32_t i = 0; i < kInstanceCount_; ++i) {
		if (ImGui::TreeNode(std::string("tramsform // " + std::to_string(i)).c_str())) {
			bool isChangePosition = exporter_.DragFloat3(std::string("position" + std::to_string(i)).c_str(), &positions_[i].x, 0.02f);
			bool isChangeScale    =  exporter_.DragFloat3(std::string("scale" + std::to_string(i)).c_str(), &scales_[i].x, 0.02f);

			if (isChangePosition || isChangeScale) {
				// matrixに設定
				(*matrix_)[i].Transfer(Matrix::MakeAffine(scales_[i], kOrigin3, positions_[i]));
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::Button("parameter export")) {
		exporter_.OutputToJson();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Field class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Field::Init() {
	for (uint32_t i = 0; i < 8; ++i) {
		AddTree(i, { 0.2f * i });
	}

	SetName("field");

	ModelBehavior::model_ = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "field.obj");
	ModelBehavior::model_->ApplyMeshShader();
	ModelBehavior::renderingFlag_ = kBehaviorRender_Systematic;
	ModelBehavior::TryLoadJson();
}

void Field::Term() {
}

void Field::Update() {
	for (auto& tree : trees_) {
		tree->Update();
	}
}

void Field::AddTree(uint32_t id, DeltaTimePoint startTime) {
	std::unique_ptr<FieldTree> tree = std::make_unique<FieldTree>();
	tree->Init(id, startTime);

	SetChild(tree.get());
	trees_.emplace_back(std::move(tree));
}
