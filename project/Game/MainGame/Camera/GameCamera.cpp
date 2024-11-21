#include "GameCamera.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>

//* lib
#include <Lib/MyMath.h>

////////////////////////////////////////////////////////////////////////////////////////////
// GameCamera class methods
////////////////////////////////////////////////////////////////////////////////////////////

void GameCamera::Init() {
	camera_ = sSystemConsole->GetGameCamera();

	SetName("mainGameCamera");

	exporter_.TryLoadFromJson();

	exporter_.GetFromStash("offset", &offset_.x, 3);
	exporter_.GetFromStash("rotate", &rotate_.x, 3);
	exporter_.GetFromStash("interpolation", &interpolation, 1);
	exporter_.GetFromStash("halfway", &halfway, 1);

	camera_->GetTransformBuffer().SetParent(&transform_);
	camera_->GetTransformBuffer().transform.rotate    = rotate_;
	camera_->GetTransformBuffer().transform.translate = offset_;
	camera_->UpdateTranslate();

	dof_ = std::make_unique<VisualProcessDoF>();
	dof_->Init();
	dof_->SetToConsole("player forcus dof");
	dof_->GetParameter().f = 8.0f;
}

void GameCamera::Term() {
}

void GameCamera::Update(PlayerManager* player, BossManager* boss) {
	Vector3f playerPosition = player->GetOperator()->get_transform().GetWorldPosition();
	Vector3f bossPosition   = boss->GetBoss()->GetPosition();

	Vector3f lerp = Lerp(playerPosition, bossPosition, halfway);
	target_ = lerp;
	target_.y = std::max(Length(bossPosition - playerPosition), 12.0f);

	transform_.transform.translate = Lerp(transform_.transform.translate, target_, interpolation);

	transform_.UpdateMatrix();

	camera_->GetTransformBuffer().transform.rotate
		= CalculateEuler(Normalize(lerp - camera_->GetTransformBuffer().GetWorldPosition()));
	camera_->UpdateTranslate();

	dof_->SetForcus(camera_, player->GetOperator()->get_transform().GetWorldPosition());
}

void GameCamera::SetAttributeImGui() {
	exporter_.DragFloat3("offset", &offset_.x, 0.01f);
	exporter_.DragFloat3("rotate", &rotate_.x, 0.01f);
	exporter_.DragFloat("interpolation", &interpolation, 0.001f);
	exporter_.SliderFloat("halfway", &halfway, 0.0f, 1.0f);

	//camera_->GetTransformBuffer().transform.rotate    = rotate_;
	camera_->GetTransformBuffer().transform.translate = offset_;
	camera_->UpdateTranslate();

	if (ImGui::Button("output parameter")) {
		exporter_.OutputToJson();
	}
}
