#include "GameCamera.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>

//* lib
#include <Lib/MyMath.h>
#include <Lib/Adapter/Random/Random.h>

////////////////////////////////////////////////////////////////////////////////////////////
// GameCamera class methods
////////////////////////////////////////////////////////////////////////////////////////////

void GameCamera::Init() {
	camera_ = sSystemConsole->GetGameCamera();

	SetName("mainGameCamera");

	exporter_.TryLoadFromJson();

	exporter_.GetFromStash("offset", &offset_.x, 3);
	exporter_.GetFromStash("interpolation", &interpolation, 1);
	exporter_.GetFromStash("halfway", &halfway, 1);

	offsetTransform_.SetParent(&transform_);
	offsetTransform_.transform.translate = offset_;
	offsetTransform_.UpdateMatrix();

	camera_->GetTransformBuffer().SetParent(&offsetTransform_);
	camera_->GetTransformBuffer().transform.translate = kOrigin3;
	camera_->UpdateTranslate();

	dof_ = std::make_unique<VisualProcessDoF>();
	dof_->Init();
	dof_->SetToConsole("player forcus dof");
	dof_->GetParameter().f = 8.0f;

	/*toon_ = std::make_unique<VisualProcessToon>();
	toon_->Init();
	toon_->SetToConsole("test toon");*/
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
	offsetTransform_.UpdateMatrix();

	Vector3f direction = Normalize(lerp - offsetTransform_.GetWorldPosition());

	offsetTransform_.transform.rotate
		= Slerp(offsetTransform_.transform.rotate, ToQuaternion(CalculateEuler(direction)), interpolation);

	offsetTransform_.UpdateMatrix();

	UpdateShake();

	camera_->GetTransformBuffer().transform.translate
		= Lerp(camera_->GetTransformBuffer().transform.translate, shakeTarget_, 0.08f);

	camera_->UpdateTranslate();

	dof_->SetForcus(camera_, player->GetOperator()->get_transform().GetWorldPosition());
}

void GameCamera::SetAttributeImGui() {
	exporter_.DragFloat3("offset", &offset_.x, 0.01f);
	exporter_.DragFloat("interpolation", &interpolation, 0.001f);
	exporter_.SliderFloat("halfway", &halfway, 0.0f, 1.0f);

	ImGui::DragFloat3("target", &target_.x, 0.01f);

	offsetTransform_.transform.translate = offset_;

	if (ImGui::Button("output parameter")) {
		exporter_.OutputToJson();
	}
}

void GameCamera::SetShake(DeltaTimePoint time, int32_t subdivision, float strength) {
	shakeTimer_    = time;
	shakeTime_     = time;
	divisionTime_ = { time.time / subdivision };
	divisionPoint_ = subdivision;

	strength_ = { Random::Generate(-strength, strength), Random::Generate(-strength, strength) };
}

void GameCamera::UpdateShake() {

	if (Sxavenger::IsTriggerKey(DIK_SPACE)) {
		SetShake({ 0.3f }, 6, 20.0f);
	}

	shakeTimer_.SubtractDeltaTime();

	if (shakeTimer_.time <= 0.0f || divisionTime_.time <= 0.0f) {
		shakeTarget_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	int32_t division = static_cast<int32_t>(shakeTimer_.time / divisionTime_.time);

	if (division < divisionPoint_) {
		divisionPoint_ = division;
		switchFlag_ = !switchFlag_;
	}

	Vector2f fixedStrength = {
		std::lerp(0.0f, strength_.x, shakeTimer_.time / shakeTime_.time),
		std::lerp(0.0f, strength_.y, shakeTimer_.time / shakeTime_.time),
	};

	if (switchFlag_) {
		shakeTarget_ = { fixedStrength.x, fixedStrength.y, 0.0f };

	} else {
		shakeTarget_ = { -fixedStrength.x, -fixedStrength.y, 0.0f };
	}
	
	//shakeTimer_.SubtractDeltaTime();
	//shakeTimer_.time = std::max(shakeTimer_.time, 0.0f);

	//float strength = 2.0f * (shakeTimer_.time / shakeTime_.time);

	//// test
	//camera_->GetTransformBuffer().transform.translate = { Random::Generate(-strength, strength), Random::Generate(-strength, strength), 0.0f };
}
