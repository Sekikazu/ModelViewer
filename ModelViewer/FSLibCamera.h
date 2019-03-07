#pragma once
#include "FSLibStructure.h"

class CAMERA {
private:
	XMMATRIX viewMatrix, projMatrix;
	float nearZ, farZ, fov, aspect;
	D3D11_VIEWPORT viewPort;

public:
	bool Init(float wndWidth, float wndHeight);

	bool SetNearFar(float nearZ, float farZ);
	bool SetPositionAndTarget_UpVecY(VECTOR eye, VECTOR focus);
	bool SetPositionAndTargetAndUpVec(VECTOR eye, VECTOR focus, VECTOR up);
	bool SetPositionAndAngle(VECTOR position, float vRotate, float hRotate, float tRotate);
	bool SetViewMatrix(MATRIX viewMatrix);
	bool SetPerspective(float fov = PI / 3.f);
	bool SetupOrtho(float size);	/* sizeはターゲットビューの縦に映す大きさ */
	bool SetupProjectionMatrix(MATRIX projectionMatrix);
	bool SetDotAspect(float dotAspect = 4.f / 3.f);
	void SetDrawArea(D3D11_VIEWPORT&);
	void SetDrawArea(float minX, float minY, float width, float height);

	XMVECTOR& GetEyePosition_XM(void);
	XMVECTOR& GetEyeDirection_XM(void);

	MATRIX& GetViewMatrix(void);
	XMMATRIX& GetViewMatrix_XM(void);
	MATRIX& GetProjectionMatrix(void);
	XMMATRIX& GetProjectionMatrix_XM(void);

	D3D11_VIEWPORT GetViewPort(void);
};

bool FSCameraInit(ID3D11Device*);