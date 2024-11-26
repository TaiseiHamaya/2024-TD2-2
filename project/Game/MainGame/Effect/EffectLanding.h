#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/ModelInstanceBehavior.h>

//* engine
#include <Engine/System/Performance.h>

////////////////////////////////////////////////////////////////////////////////////////////
// EffectLanding class
////////////////////////////////////////////////////////////////////////////////////////////
class EffectLanding
	: public ModelInstanceBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	EffectLanding()  = default;
	~EffectLanding() { Term(); }

	void Init();

	void Term();

	void Update();

	void DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) override;

	void CreateParticle(const Vector3f& position, const Vector3f& velocity);

private:

	////////////////////////////////////////////////////////////////////////////////////////////
	// Element structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct Element {
		QuaternionTransform transform;
		Vector3f velocity;

		DeltaTimePoint aliveTime;
		DeltaTimePoint aliveTimer = {};
	};

	//=========================================================================================
	// private variables
	//=========================================================================================

	static const uint32_t kInstanceCount_ = 128;
	uint32_t instanceIndex_ = 0;

	std::list<Element> elements_;

};