#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/ModelBehavior.h>

//* engine
#include <Engine/System/Performance.h>

class ClearSprite
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	ClearSprite() = default;
	~ClearSprite() = default;

	void Init();

	void Term();

	void Update(const Vector3f& position, const Quaternion& quaternion);

	void DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) override;

	size_t GetExplainSize() const { return textures_.size(); }

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	Vector3f offset_ = { -1.0f, 2.0f, 0.0f };

	std::array<Texture*, 2> textures_ = {};
	std::array<Texture*, 2> texturesKey_ = {};

	DeltaTimePoint displayTimer_ = {};

	DeltaTimePoint openAndCloseTime_ = { 1.0f };
	DeltaTimePoint openAndCloseTimer_;

	uint32_t textureIndex_ = 0;
};

