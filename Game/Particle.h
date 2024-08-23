#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// DxObject
#include <DxShaderBlob.h>

// CS
#include <DxCSPipeline.h>
#include <DxUnorderedBufferResource.h>

// Graphics
#include <DxGraphicsPipeline.h>
#include <DxShaderReflection.h>

// IA
#include <Model.h>
#include <DrawMethod.h>

// c++
#include <memory>

// Geometry
#include <Vector4.h>
#include <Vector3.h>
#include <Matrix4x4.h>

// rendering
#include <DefferedRendering.h>


// attribtue
#include <Attribute.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Particle class
////////////////////////////////////////////////////////////////////////////////////////////
class Particle
	: public Attribute {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	Particle() = default;

	~Particle() { Term(); }

	void Init();

	void Term();

	void Update();

	void Draw();

	void SetAttributeImGui() override;

private:

	////////////////////////////////////////////////////////////////////////////////////////////
	// ParticleCS structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct ParticleCS {
		Color4f color;
		Vector3f scale;
		Vector3f translate;
		Vector3f velocity;
		float currentTime;
		float lifeTime;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	// EmitterSphere structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct EmitterSphere {
		Vector3f position;   //!< 位置
		float radius;        //!< 射出半径
		uint32_t count;      //!< 発射数
		float frequency;     //!< 発射間隔
		float frequencyTime; //!< 発射間隔調整用時間
		int isEmit;          //!< 発射許可(bool)
	};

	//=========================================================================================
	// private variables
	//=========================================================================================

	//* CS *//
	std::unique_ptr<DxObject::CSBlob>          csInitBlob_;
	std::unique_ptr<DxObject::CSPipeline> csInitPipeline_;

	std::unique_ptr<DxObject::CSBlob>          csBlob_;
	std::unique_ptr<DxObject::CSPipeline> csPipeline_;

	std::unique_ptr<DxObject::CSBlob>          csUpdateBlob_;
	std::unique_ptr<DxObject::CSPipeline> csUpdatePipeline_;

	//* buffers *//
	static const uint32_t kParticleNum = 1024;

	std::unique_ptr<DxObject::UnorderedBufferResource<ParticleCS>> particleBuffer_;
	std::unique_ptr<DxObject::BufferResource<uint32_t>>     informationBuffer_;
	std::unique_ptr<DxObject::UnorderedBufferResource<int32_t>>    counterBuffer_;

	std::unique_ptr<DxObject::BufferResource<EmitterSphere>> emitterBuffer_;

	//* Graphics *//
	std::unique_ptr<DxObject::GraphicsBlob>          blob_;
	std::unique_ptr<DxObject::ShaderReflectionTable> reflection_;
	std::unique_ptr<DxObject::GraphicsPipeline>      pipeline_;

	//* IA *//
	DrawData plane_;

};
