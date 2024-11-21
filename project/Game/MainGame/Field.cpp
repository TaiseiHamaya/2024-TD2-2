#include "Field.h"

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
}

void FieldTree::Term() {
}

void FieldTree::Update() {
	animationTimer_.AddDeltaTime();
	animationTimer_.time = std::fmod(animationTimer_.time, animator_->GetAnimationDuration(0).time);

	animator_->Update(animationTimer_, 0, true);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Field class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Field::Init() {
	for (uint32_t i = 0; i < 8; ++i) {
		AddTree(i, { 0.4f * i });
	}

	SetName("field");
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
