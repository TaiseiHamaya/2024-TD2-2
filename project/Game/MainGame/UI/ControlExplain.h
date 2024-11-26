#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/ModelBehavior.h>

//* engine
#include <Engine/System/Performance.h>

////////////////////////////////////////////////////////////////////////////////////////////
// ControlExplain class
////////////////////////////////////////////////////////////////////////////////////////////
class ControlExplain
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	ControlExplain()  = default;
	~ControlExplain() = default;

	void Init();

	void Term();

	void Update(const Vector3f& position, const Quaternion& quaternion);

	void DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) override;

	void SetExplainParameter(DeltaTimePoint displayTime, uint32_t textureIndex);

	bool IsDisplayExplain() const;

	size_t GetExplainSize() const { return textures_.size(); }

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	Vector3f offset_ = { 0.0f, 8.0f, 0.0f };

	std::array<Texture*, 2> textures_ = {};

	DeltaTimePoint displayTimer_ = {};

	DeltaTimePoint openAndCloseTime_  = { 1.0f };
	DeltaTimePoint openAndCloseTimer_;

	uint32_t textureIndex_ = 0;

};