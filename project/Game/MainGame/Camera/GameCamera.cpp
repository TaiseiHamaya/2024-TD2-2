#include "GameCamera.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>

//* lib
#include <Lib/MyMath.h>
#include <Lib/Easing.h>
#include <Lib/Adapter/Random/Random.h>

//* game
#include <Game/MainGame/Boss/BossManager.h>
#include <Game/MainGame/Player/PlayerManager.h>

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
	dof_->GetParameter().f = 40.0f;

	/*toon_ = std::make_unique<VisualProcessToon>();
	toon_->Init();
	toon_->SetToConsole("test toon");*/
}

void GameCamera::Term() {
}

void GameCamera::Update(PlayerManager* player, BossManager* boss) {

	dynamicHalfway_ = halfway;

	UpdateZoom(player);

	UpdateTarget(player, boss);

	offsetTransform_.UpdateMatrix();

	UpdateShake();

	camera_->UpdateTranslate();
	camera_->SetProjection({ 16.0f, 9.0f }, forcusLength_, 0.01f, 1000.0f);

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

	/*if (Sxavenger::IsTriggerKey(DIK_SPACE)) {
		SetShake({ 0.3f }, 6, 20.0f);
	}*/

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

	camera_->GetTransformBuffer().transform.translate
		= Lerp(camera_->GetTransformBuffer().transform.translate, shakeTarget_, 0.08f);
}

void GameCamera::UpdateTarget(PlayerManager* player, BossManager* boss) {

	Vector3f playerPosition = player->GetOperator()->get_transform().GetWorldPosition();
	Vector3f bossPosition   = boss->GetBoss()->GetPosition();

	Vector3f lerp = Lerp(playerPosition, bossPosition, halfway);
	target_ = lerp;
	target_.y = std::max(Length(bossPosition - playerPosition), 23.0f);

	transform_.transform.translate = Lerp(transform_.transform.translate, target_, interpolation);
	transform_.UpdateMatrix();
	offsetTransform_.UpdateMatrix();

	Vector3f direction = Normalize(lerp - offsetTransform_.GetWorldPosition());

	offsetTransform_.transform.rotate
		= Slerp(offsetTransform_.transform.rotate, ToQuaternion(CalculateEuler(direction)), interpolation);
}

void GameCamera::UpdateZoom(PlayerManager* player) {

	auto gamepad = Sxavenger::GetInput()->GetGamepadInput(0);
	bool isPress = gamepad->IsPressButton(XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_LEFT_SHOULDER);

	if (isPress && player->CanShot()) {
		zoomTimer_.AddDeltaTime();
		zoomTimer_.time = std::clamp(zoomTimer_.time, 0.0f, zoomTime_.time);

		float t = std::lerp(0.0f, 0.2f, zoomTimer_.time / zoomTime_.time);

		forcusLength_ = std::lerp(forcusLength_, 24.0f, t);

		if (zoomTimer_ == zoomTime_) {
			SetShake({0.1f}, 2, 1.0f);
		}

	} else {

		zoomTimer_.SubtractDeltaTime();
		zoomTimer_.time = std::clamp(zoomTimer_.time, 0.0f, zoomTime_.time);

		float t = std::lerp(0.0f, 0.2f, 1.0f - zoomTimer_.time / zoomTime_.time);

		forcusLength_ = std::lerp(forcusLength_, 20.0f, t);
	}
	
}
