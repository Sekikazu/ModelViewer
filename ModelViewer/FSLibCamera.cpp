#include "FSLibCamera.h"

bool CAMERA::Init(float wndW, float wndH) {
	// ViewPort
	viewPort;
	viewPort.Width = wndW;
	viewPort.Height = wndH;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->RSSetViewports(1, &viewPort);

	// 
	XMVECTOR eye = XMVectorSet(2.f, 1.f, 1.2f, 0.f);
	XMVECTOR focus = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	fov = XMConvertToRadians(45.f);
	aspect = wndW / wndH;
	nearZ = 0.1f;
	farZ = 100.f;
	projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

	return true;
}

bool CAMERA::SetNearFar(float n, float f) {
	if (n >= f)return false;

	nearZ = n;
	farZ = f;
	projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

	return true;
}

bool CAMERA::SetPositionAndTarget_UpVecY(VECTOR eye, VECTOR focus) {
	viewMatrix = XMMatrixLookAtLH(eye, focus, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	return true;
}

bool CAMERA::SetPositionAndTargetAndUpVec(VECTOR eye, VECTOR focus, VECTOR up) {
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	return true;
}

bool CAMERA::SetViewMatrix(MATRIX viewMat) {
	viewMatrix = viewMat;
	return true;
}

bool CAMERA::SetPerspective(float f) {
	if (f < 0)return false;
	fov = f;
	projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	return true;
}

bool CAMERA::SetDotAspect(float dotAspect) {
	aspect = dotAspect;
	projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	return true;
}

void CAMERA::SetDrawArea(D3D11_VIEWPORT& vp) {
	extern ID3D11DeviceContext* DeviceContext;
	viewPort = vp;
	
	DeviceContext->RSSetViewports(1, &viewPort);
}

void CAMERA::SetDrawArea(float minX, float minY, float width, float height) {
	viewPort.TopLeftX = minX;
	viewPort.TopLeftY = minY;
	viewPort.Width = width;
	viewPort.Height = height;
	
	SetDrawArea(viewPort);
}

XMVECTOR& CAMERA::GetEyePosition_XM(void) {
	XMVECTOR buf;
	buf.m128_f32[0] = -XMVector3Dot(viewMatrix.r[0], viewMatrix.r[3]).m128_f32[0];
	buf.m128_f32[1] = -XMVector3Dot(viewMatrix.r[1], viewMatrix.r[3]).m128_f32[0];
	buf.m128_f32[2] = -XMVector3Dot(viewMatrix.r[2], viewMatrix.r[3]).m128_f32[0];
	buf.m128_f32[3] = 0.f;
	return buf;
}

XMVECTOR& CAMERA::GetEyeDirection_XM(void) {
	XMVECTOR buf;
	buf.m128_f32[0] = viewMatrix.r[0].m128_f32[2];
	buf.m128_f32[1] = viewMatrix.r[1].m128_f32[2];
	buf.m128_f32[2] = viewMatrix.r[2].m128_f32[2];
	buf.m128_f32[3] = 0.f;
	return buf;
}

MATRIX& CAMERA::GetViewMatrix(void) { return XMMatToMat(viewMatrix); }
XMMATRIX& CAMERA::GetViewMatrix_XM(void) { return viewMatrix; }
MATRIX& CAMERA::GetProjectionMatrix(void) { return XMMatToMat(projMatrix); }
XMMATRIX& CAMERA::GetProjectionMatrix_XM(void) { return projMatrix; }
D3D11_VIEWPORT CAMERA::GetViewPort(void) { return viewPort; }
