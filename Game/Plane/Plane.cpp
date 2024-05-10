#include "Plane.h"

#include "MyEngine.h"
#include "Console.h"

using namespace DxObject;

void Plane::Init() {

	SetThisAttribute("Plane");

	vertexResource_ = std::make_unique<BufferResource<VertexData>>(MyEngine::GetDevicesObj(), 4);
	vertexResource_->operator[](0).position = { -1.0f, -1.0f, 0.0f };
	vertexResource_->operator[](0).texcoord = { 0.0f, 1.0f };
	vertexResource_->operator[](1).position = { 1.0f, -1.0f, 0.0f };
	vertexResource_->operator[](1).texcoord = { 1.0f, 1.0f };
	vertexResource_->operator[](2).position = { 1.0f, 1.0f, 0.0f };
	vertexResource_->operator[](2).texcoord = { 1.0f, 0.0f };
	vertexResource_->operator[](3).position = { -1.0f, 1.0f, 0.0f };
	vertexResource_->operator[](3).texcoord = { 0.0f, 0.0f };
	

	for (uint32_t i = 0; i < vertexResource_->GetIndexSize(); ++i) {
		vertexResource_->operator[](i).normal = { 0.0f, 0.0f, 1.0f };
	}

	indexResource_ = std::make_unique<IndexBufferResource>(MyEngine::GetDevicesObj(), 6);
	indexResource_->operator[](0) = 0;
	indexResource_->operator[](1) = 3;
	indexResource_->operator[](2) = 1;
	indexResource_->operator[](3) = 3;
	indexResource_->operator[](4) = 2;
	indexResource_->operator[](5) = 1;

	materialResource_ = std::make_unique<BufferPtrResource<PlaneMaterial>>(MyEngine::GetDevicesObj(), 1);
	materialResource_->SetPtr(0, &material_);
	material_.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	matrixResource_ = std::make_unique<BufferResource<TransformationMatrix>>(MyEngine::GetDevicesObj(), 1);
	matrixResource_->operator[](0).world = Matrix4x4::MakeIdentity();
	matrixResource_->operator[](0).wvp   = Matrix4x4::MakeIdentity();
	matrixResource_->operator[](0).worldInverseTranspose = Matrix4x4::MakeIdentity();

}

void Plane::Term() {
	vertexResource_.reset();
	indexResource_.reset();
	matrixResource_.reset();
	materialResource_.reset();
}

void Plane::Update() {
	// matrix
	Matrix4x4 world = Matrix::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);

	matrixResource_->operator[](0).world = world;
	matrixResource_->operator[](0).wvp = world * MyEngine::camera3D_->GetViewProjectionMatrix();
	matrixResource_->operator[](0).worldInverseTranspose = world;
}

void Plane::Draw() {
	MyEngine::SetBlendMode(kBlendModeNormal);
	MyEngine::SetPipelineType(FLOOR);
	MyEngine::SetPipelineState();

	// commandListの取り出し
	auto commandList = MyEngine::GetCommandList();

	// ParamBuffers
	commandList->SetGraphicsRootConstantBufferView(0, matrixResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, MyEngine::GetTextureHandleGPU("resources/uvChecker.png"));


	// mesh param
	commandList->SetGraphicsRootShaderResourceView(3, vertexResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootShaderResourceView(4, indexResource_->GetGPUVirtualAddress());

	commandList->DispatchMesh(1, 1, 1);
}

void Plane::SetAttributeImGui() {
	ImGui::DragFloat3("scale",     &transform_.scale.x,     0.01f);
	ImGui::DragFloat3("rotate",    &transform_.rotate.x,    0.01f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
}