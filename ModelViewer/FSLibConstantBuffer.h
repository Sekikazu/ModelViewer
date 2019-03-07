#pragma once
#include "FSLibStructure.h"
#include "FSLibCamera.h"
#include "FSLibLight.h"

class CONSTANT_MANAGER {
private:
	// 方向ライト
	CONSTANT_BUFFER_DIRECTIONALLIGHT directional;
	ID3D11Buffer* pDirectional;
	UINT directionalSize;

	// ポイントライト
	CONSTANT_BUFFER_POINTLIGHT point;
	ID3D11Buffer* pPoint;
	UINT pointSize;

public:
	void Init(ID3D11Device*);

	CONSTANT_BUFFER CreateConstantBuffer(size_t size);
	bool SetConstant(CONSTANT_BUFFER* constantBuffer, void* constant);

	bool SetConstant_Directional(XMMATRIX& worldMat, int lightN, ID3D11Buffer** constantBuffer, size_t* constantSize);
	bool SetConstant_Point(XMMATRIX& worldMat, int lightN, ID3D11Buffer** constantBuffer, size_t* constantSize);
};

