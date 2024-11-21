#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* base
#include <Engine/Game/Behavior/AnimationBehavior.h>

//* engine
#include <Engine/Game/SxavengerPipeline/VisualProcessLayer.h>

//* c++
#include <list>
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////
// FieldTree class
////////////////////////////////////////////////////////////////////////////////////////////
class FieldTree
	: public AnimationBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	FieldTree()  = default;
	~FieldTree() = default;

	void Init(uint32_t id, DeltaTimePoint startTime);

	void Term();

	void Update();

	void DrawSystematic(_MAYBE_UNUSED const Camera3D* camera) override;

	void SetAttributeImGui() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::unique_ptr<Animator> animator_;
	DeltaTimePoint animationTimer_;

	static const uint32_t kInstanceCount_ = 4;

	std::unique_ptr<DxObject::BufferResource<TransformationMatrix>> matrix_;

	Vector3f positions_[kInstanceCount_] = {};
	Vector3f scales_[kInstanceCount_] = {};

};

////////////////////////////////////////////////////////////////////////////////////////////
// Field class
////////////////////////////////////////////////////////////////////////////////////////////
class Field
	: public ModelBehavior {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	Field()  = default;
	~Field() { Term(); }

	void Init();

	void Term();

	void Update();

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::list<std::unique_ptr<FieldTree>> trees_;
	
	//=========================================================================================
	// private methods
	//=========================================================================================

	void AddTree(uint32_t id, DeltaTimePoint startTime);

};