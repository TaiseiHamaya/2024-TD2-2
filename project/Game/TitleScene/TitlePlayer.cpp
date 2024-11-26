#include "TitlePlayer.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Game/SxavengerGame.h>
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// TitlePlayer class
////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::Init() {
	SetName("titlePlayer");

	AnimationBehavior::model_ = SxavengerGame::LoadModel("resourcesData/GameScene/Model", "player_move.gltf");

	animator_                    = std::make_unique<Animator>(model_);
	AnimationBehavior::animator_ = animator_.get();
	
	AnimationBehavior::renderingFlag_ = kBehaviorRender_Systematic;

	AnimationBehavior::TryLoadJson();

	dof_ = std::make_unique<VisualProcessDoF>();
	dof_->Init();
	dof_->SetToConsole("title dof");
	dof_->GetParameter().focusLength = 22.9f;
	dof_->GetParameter().f           = 16.0f;

	explain_ = std::make_unique<ControlExplain>();
	explain_->Init();
	SetChild(explain_.get());
}

void TitlePlayer::Term() {
}

void TitlePlayer::Update() {

	animationTime_.AddDeltaTime();

	for (uint32_t i = 0; i < animator_->GetAnimationSize(); ++i) {
		animator_->Update(animationTime_, i);
	}

	if (!explain_->IsDisplayExplain()) {
		explain_->SetExplainParameter({ 2.0f }, explainIndex_);
		explainIndex_++;
		explainIndex_ %= explain_->GetExplainSize();
	}

	explain_->Update(transform_.GetWorldPosition(), sSystemConsole->GetGameCamera()->GetTransform().rotate);

}
