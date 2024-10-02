#include "Camera3D.h"
_DXOBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// Lib
#include <Lib/Environment.h>

// engine
#include <Engine/System/Sxavenger.h>
//#include <SxavengerGraphics.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Camera3D class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Camera3D::Init() {

	// bufferの設定
	buffer_ = std::make_unique<BufferResource<CameraForGPU>>(Sxavenger::GetDevicesObj(), 1);

	// cameraの設定
	SetTransform(kUnit3, kOrigin3, {0.0f, 0.0f, -10.0f});
	SetProjection(0.45f, static_cast<float>(kWindowSize.x) / static_cast<float>(kWindowSize.y), 0.1f, 100.0f);

}

void Camera3D::Term() {
}

void Camera3D::SetTransform(const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate) {

	transform_.scale     = scale;
	transform_.rotate    = rotate;
	transform_.translate = translate;

	CalculateView();
}

void Camera3D::SetProjection(float fovY, float aspectRatio, float nearClip, float farClip) {
	(*buffer_)[0].projMatrix = Matrix::MakePerspectiveFov(fovY, aspectRatio, nearClip, farClip);
}

void Camera3D::DrawFrustum(const Color4f& color) const {

	Vector3f frustumPoint[4] = {};
	Matrix4x4 clipMatrix  = (*buffer_)[0].projMatrix.Inverse();
	Matrix4x4 worldMatrix = (*buffer_)[0].viewMatrix.Inverse();

	frustumPoint[0] = Matrix::Transform(Matrix::Transform({ -1.0f, -1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[1] = Matrix::Transform(Matrix::Transform({ -1.0f, 1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[2] = Matrix::Transform(Matrix::Transform({ 1.0f, 1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[3] = Matrix::Transform(Matrix::Transform({ 1.0f, -1.0f, 1.0f }, clipMatrix), worldMatrix);

	for (int i = 0; i < 4; ++i) {
		/*SxavengerGraphics::DrawLine(
			frustumPoint[i], frustumPoint[(i + 1) % 4], color
		);

		SxavengerGraphics::DrawLine(
			frustumPoint[i], transform_.translate, color
		);*/

		color;
	}
}

void Camera3D::CalculateView() {

	// positionの代入
	(*buffer_)[0].position = { transform_.translate.x, transform_.translate.y, transform_.translate.z };

	// viewMatrixの代入
	(*buffer_)[0].viewMatrix = transform_.ToMatrix().Inverse();

}


void Camera3D::SetAttributeImGui() {
	transform_.SetImGuiCommand();
	CalculateView();
}