#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/BaseBehavior.h>

//* engine
#include <Engine/Game/Camera/CineCamera.h>
#include <Engine/System/Performance.h>
#include <Engine/Game/SxavengerPipeline/VisualProcessLayer.h>

class PlayerManager;
class BossManager;

////////////////////////////////////////////////////////////////////////////////////////////
// GameCamera class
////////////////////////////////////////////////////////////////////////////////////////////
class GameCamera
	: public BaseBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	GameCamera()  = default;
	~GameCamera() { Term(); }

	void Init();

	void Term();

	void Update(PlayerManager* player, BossManager* boss);

	void SetAttributeImGui() override;

	void SetShake(DeltaTimePoint time, int32_t subdivision, float strength);

private:

	//=========================================================================================
	// private varibales
	//=========================================================================================

	//* external *//

	CineCamera* camera_ = nullptr; 

	QuaternionTransformBuffer transform_;
	QuaternionTransformBuffer offsetTransform_;

	Vector3f target_ = {};

	Vector3f offset_ = { 0.0f, 8.0f, -4.0f };
	float interpolation = 0.02f;
	float halfway = 0.5f;

	float dynamicHalfway_ = 0.0f;
	float forcusLength_   = 20.0f;

	DeltaTimePoint zoomTime_  = { 2.0f };
	DeltaTimePoint zoomTimer_ = {};

	Quaternion targetQuaterion_  = Quaternion::Identity();

	//* shake *//

	DeltaTimePoint shakeTimer_   = {};
	DeltaTimePoint shakeTime_    = {};
	DeltaTimePoint divisionTime_ = {};
	int32_t divisionPoint_ = 0;

	bool switchFlag_ = false;

	Vector2f strength_ = {};

	Vector3f shakeTarget_ = {};

	//* dof *//

	std::unique_ptr<VisualProcessDoF> dof_;
	std::unique_ptr<VisualProcessToon> toon_;

	// 視野角狭め, 座標高め

	//=========================================================================================
	// private methods
	//=========================================================================================

	void UpdateShake();
	void UpdateTarget(PlayerManager* player, BossManager* boss);
	void UpdateZoom(PlayerManager* player);

};