#include "FSLibConstantBuffer.h"

void CONSTANT_MANAGER::Init(ID3D11Device* Device) {
	D3D11_BUFFER_DESC cbDesc;
	directionalSize = cbDesc.ByteWidth = sizeof(CONSTANT_BUFFER_DIRECTIONALLIGHT);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	Device->CreateBuffer(&cbDesc, NULL, &pDirectional);

	pointSize = cbDesc.ByteWidth = sizeof(CONSTANT_BUFFER_POINTLIGHT);
	Device->CreateBuffer(&cbDesc, NULL, &pPoint);
}

CONSTANT_BUFFER CONSTANT_MANAGER::CreateConstantBuffer(size_t size) {
	extern ID3D11Device* Device;
	CONSTANT_BUFFER cb;
	cb.bufferSize = size;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = cb.bufferSize;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	Device->CreateBuffer(&cbDesc, NULL, &cb.buffer);

	return cb;
}

bool CONSTANT_MANAGER::SetConstant(CONSTANT_BUFFER* constantBuffer, void* constant) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->UpdateSubresource(constantBuffer->buffer, 0, NULL, constant, 0, 0);
	return true;
}


bool CONSTANT_MANAGER::SetConstant_Directional(XMMATRIX& worldMat, int lightN, ID3D11Buffer** constantBuffer, size_t* constantSize) {
	bool f;

	extern ID3D11DeviceContext* DeviceContext;
	extern CAMERA Camera;
	extern LIGHT Light;

	DirectX::XMStoreFloat4x4(&directional.world, XMMatrixTranspose(worldMat));
	DirectX::XMStoreFloat4x4(&directional.view, XMMatrixTranspose(Camera.GetViewMatrix_XM()));
	DirectX::XMStoreFloat4x4(&directional.projection, XMMatrixTranspose(Camera.GetProjectionMatrix_XM()));

	DIRECTIONAL_LIGHT dl;
	if (!(f = Light.GetDirectional(lightN, &dl)))return false;
	DirectX::XMStoreFloat4(&directional.lightDirection, XMVECTOR(dl.direction));
	directional.lightColor.x = dl.color.col[0];
	directional.lightColor.y = dl.color.col[1];
	directional.lightColor.z = dl.color.col[2];
	directional.lightColor.w = dl.color.col[3];

	XMFLOAT4 eyeDirection;
	XMStoreFloat4(&eyeDirection, Camera.GetEyeDirection_XM());
	directional.eyeDirection = eyeDirection;

	DeviceContext->UpdateSubresource(pDirectional, 0, NULL, &directional, 0, 0);

	*constantBuffer = pDirectional;
	*constantSize = directionalSize;

	return true;
}

bool CONSTANT_MANAGER::SetConstant_Point(XMMATRIX& worldMat, int lightN, ID3D11Buffer** constantBuffer, size_t* constantSize) {
	bool f;

	extern ID3D11DeviceContext* DeviceContext;
	extern CAMERA Camera;
	extern LIGHT Light;

	POINT_LIGHT pl;
	f = Light.GetPoint(lightN, &pl);
	if (!f)return false;
	DirectX::XMStoreFloat4x4(&point.world, XMMatrixTranspose(worldMat));
	DirectX::XMStoreFloat4x4(&point.view, XMMatrixTranspose(Camera.GetViewMatrix_XM()));
	DirectX::XMStoreFloat4x4(&point.projection, XMMatrixTranspose(Camera.GetProjectionMatrix_XM()));
	DirectX::XMStoreFloat4(&point.lightPos, XMVECTOR(pl.pos));
	VECTOR atten = VGet(pl.atten[0], pl.atten[1], pl.atten[2]);
	DirectX::XMStoreFloat4(&point.lightPal, XMVECTOR(atten));

	DeviceContext->UpdateSubresource(pPoint, 0, NULL, &point, 0, 0);

	*constantBuffer = pPoint;
	*constantSize = pointSize;

	return true;
}