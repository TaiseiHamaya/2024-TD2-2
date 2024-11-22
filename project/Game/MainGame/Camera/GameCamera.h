#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/BaseBehavior.h>

//* engine
#include <Engine/Game/Camera/CineCamera.h>

//* game
#include <Game/MainGame/Boss/BossManager.h>
#include <Game/MainGame/Player/PlayerManager.h>

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

	//void SetShake(DeltaTimePoint time, divisionPoint_);

private:

	//=========================================================================================
	// private varibales
	//=========================================================================================

	//* external *//

	CineCamera* camera_ = nullptr;

	QuaternionTransformBuffer transform_;

	Vector3f target_ = {};

	Vector3f offset_ = { 0.0f, 8.0f, -4.0f };
	float interpolation = 0.02f;
	float halfway = 0.5f;

	Quaternion targetQuaterion_  = Quaternion::Identity();

	//* shake *//

	DeltaTimePoint shakeTime_ = {};
	int32_t subdivision_;
	int32_t divisionPoint_ = 0;

	//* dof *//

	std::unique_ptr<VisualProcessDoF> dof_;
	std::unique_ptr<VisualProcessToon> toon_;

	// 視野角狭め, 座標高め
};