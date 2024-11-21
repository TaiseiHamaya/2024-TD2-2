#include "VisualProcessLayer.h"
_DXOBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* engine
#include <Engine/Console/SystemConsole.h>

////////////////////////////////////////////////////////////////////////////////////////////
// BaseVisualProcessLayer class methods
////////////////////////////////////////////////////////////////////////////////////////////

void BaseVisualProcessLayer::Term() {
	sSystemConsole->RemoveVisualLayer(this);
}

void BaseVisualProcessLayer::SetToConsole() {
	sSystemConsole->SetVisualLayer(this);
}

void BaseVisualProcessLayer::SetToConsole(const std::string& name) {
	SetName(name);
	SetToConsole();
}

////////////////////////////////////////////////////////////////////////////////////////////
// VisualProcessDoF class methods
////////////////////////////////////////////////////////////////////////////////////////////

void VisualProcessDoF::Init() {
	parameter_ = std::make_unique<BufferResource<Parameter>>(Sxavenger::GetDevicesObj(), 1);

	// 仮値
	(*parameter_)[0].f           = 0.01f;
	(*parameter_)[0].focusLength = 0.01f;
}

void VisualProcessDoF::Term() {
}

void VisualProcessDoF::Process(_MAYBE_UNUSED SxavengerFrame* frame) {
	frame->GetVisual()->NextResultBufferIndex();

	auto commandList = Sxavenger::GetCommandList();

	sSystemConsole->SetProcessPipeline(kVisual_DoF);

	commandList->SetComputeRootConstantBufferView(0, frame->GetConfigVirtualAddress());
	commandList->SetComputeRootDescriptorTable(1, frame->GetVisual()->GetPrevBuffer(1)->GetGPUHandleSRV());
	commandList->SetComputeRootDescriptorTable(2, frame->GetDepthBufferController()->GetRasterizeGPUHandleSRV());
	commandList->SetComputeRootConstantBufferView(3, frame->GetCamera()->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(4, parameter_->GetGPUVirtualAddress());
	commandList->SetComputeRootDescriptorTable(5, frame->GetVisual()->GetResultBuffer()->GetGPUHandleUAV());

	sSystemConsole->Dispatch(frame->GetSize());

}

void VisualProcessDoF::SetLayerImGui() {
	ImGui::CheckboxFlags("debug DoF", &(*parameter_)[0].isDebugDoF, 1);
	ImGui::DragFloat("focus length", &(*parameter_)[0].focusLength, 0.01f);
	ImGui::DragFloat("f", &(*parameter_)[0].f, 0.01f);
}

void VisualProcessDoF::SetForcus(const Camera3D* camera, const Vector3f& position) {

	Vector3f clip = Matrix::Transform(position, camera->GetViewProjMatrix());
	Vector3f viewDepth = Matrix::Transform({ 0.0f, 0.0f, clip.z }, camera->GetProjInverseMatrix());

	(*parameter_)[0].focusLength = viewDepth.z;
}
