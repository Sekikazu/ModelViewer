#pragma once
#include <DirectXTex.h>
#include <Inc\CommonStates.h>
#include <Inc\Keyboard.h>
#include <Inc\SpriteFont.h>
#include <fbxsdk.h>
#include "WICTextureLoader.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;
using namespace std;

#define PI 3.141592654
#define RELEASE(x) if(x != nullptr){x->Release(); x=nullptr;}

struct COLOR {
	float col[4];
};
inline COLOR GetColor(float r, float g, float b, float a) {
	COLOR c;
	c.col[0] = r;
	c.col[1] = g;
	c.col[2] = b;
	c.col[3] = a;
	return c;
}

struct VECTOR2D {
	float x, y;

	VECTOR2D operator+(const VECTOR2D& add) {
		VECTOR2D buf;
		buf.x = x + add.x;
		buf.y = y + add.y;
		return buf;
	}
	VECTOR2D operator-(const VECTOR2D& sub) {
		VECTOR2D buf;
		buf.x = x - sub.x;
		buf.y = y - sub.y;
		return buf;
	}
	VECTOR2D operator*(const float& scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	float operator*(const VECTOR2D& dot) {
		float buf;
		buf = x * dot.x + y * dot.y;
		return buf;
	}
	VECTOR2D& operator+=(const VECTOR2D& add) {
		x += add.x;
		y += add.y;
		return *this;
	}
	VECTOR2D& operator-=(const VECTOR2D& sub) {
		x -= sub.x;
		y -= sub.y;
		return *this;
	}
	VECTOR2D& operator*=(const float& scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	float operator*=(const VECTOR2D& dot) {
		float buf;
		buf = x * dot.x + y * dot.y;
		return buf;
	}
};
inline VECTOR2D VGet2D(float x, float y) {
	VECTOR2D buf;
	buf.x = x; buf.y = y;
	return buf;
}

struct VECTOR {
	float x, y, z;

	VECTOR operator+(const VECTOR& add) {
		VECTOR buf;
		buf.x = x + add.x;
		buf.y = y + add.y;
		buf.z = z + add.z;
		return buf;
	}
	VECTOR operator-(const VECTOR& sub) {
		VECTOR buf;
		buf.x = x - sub.x;
		buf.y = y - sub.y;
		buf.z = z - sub.z;
		return buf;
	}
	VECTOR operator*(const float& scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	float operator*(const VECTOR& dot) {
		float buf;
		buf = x * dot.x + y * dot.y + z * dot.z;
		return buf;
	}
	VECTOR& operator+=(const VECTOR& add) {
		x += add.x;
		y += add.y;
		z += add.z;
		return *this;
	}
	VECTOR& operator-=(const VECTOR& sub) {
		x -= sub.x;
		y -= sub.y;
		z -= sub.z;
		return *this;
	}
	VECTOR& operator*=(const float& scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	float operator*=(const VECTOR& dot) {
		float buf;
		buf = x * dot.x + y * dot.y + z * dot.z;
		return buf;
	}
	operator XMVECTOR() {
		XMVECTOR xmv;
		xmv.m128_f32[0] = x;
		xmv.m128_f32[1] = y;
		xmv.m128_f32[2] = z;
		xmv.m128_f32[3] = 0;
		return xmv;
	}
};
inline VECTOR VGet(float x, float y, float z) {
	VECTOR buf;
	buf.x = x; buf.y = y; buf.z = z;
	return buf;
}
inline VECTOR VGet(XMVECTOR xmV) {
	VECTOR buf = VGet(xmV.m128_f32[0], xmV.m128_f32[1], xmV.m128_f32[2]);
	return buf;
}
inline VECTOR VNorm(VECTOR vect) {
	float size = sqrtf(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
	vect.x /= size;
	vect.y /= size;
	vect.z /= size;
	return vect;
}
inline VECTOR VNorm(float x, float y, float z) {
	VECTOR buf;
	float size = sqrtf(x * x + y * y + z * z);
	buf.x = x / size; buf.y = y / size; buf.z = z / size;
	return buf;
}
inline VECTOR VCross(VECTOR a, VECTOR b) {
	VECTOR buf;
	buf.x = a.y*b.z - a.z*b.y;
	buf.y = a.z*b.x - a.x*b.z;
	buf.z = a.x*b.y - a.y*b.x;
	return buf;
}
inline D3D11_INPUT_ELEMENT_DESC  GetVectorIED(void) {
	D3D11_INPUT_ELEMENT_DESC ied = { "VECTOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	return ied;
}

struct MATRIX {
	float m[4][4];
	MATRIX operator+(const MATRIX& add) {
		MATRIX buf;
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)
			buf.m[a][b] = m[a][b] + add.m[a][b];
		return buf;
	}
	MATRIX operator-(const MATRIX& sub) {
		MATRIX buf;
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)
			buf.m[a][b] = m[a][b] - sub.m[a][b];
		return buf;
	}
	MATRIX operator*(const MATRIX& mult) {
		MATRIX buf;
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++) {
			buf.m[a][b] = 0.f;
			for (int c = 0; c < 4; c++)
				buf.m[a][b] += m[a][c] * mult.m[c][b];
		}
		return buf;
	}
	MATRIX& operator+=(const MATRIX& add) {
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)
			m[a][b] += add.m[a][b];
		return *this;
	}
	MATRIX& operator-=(const MATRIX& add) {
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)
			m[a][b] -= add.m[a][b];
		return *this;
	}
	MATRIX& operator*=(const MATRIX& mult) {
		MATRIX buf;
		ZeroMemory(&buf, sizeof(MATRIX));
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++) {
			buf.m[a][b] = 0.f;
			for (int c = 0; c < 4; c++)
				buf.m[a][b] += m[a][c] * mult.m[c][b];
		}
		for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)m[a][b] = buf.m[a][b];
		return buf;
	}
	operator XMMATRIX() {
		XMMATRIX xm;
		for(int a=0;a<4;a++)for(int b=0;b<4;b++)
			xm.r[a].m128_f32[b] = m[a][b];
		return xm;
	}
};
inline MATRIX& XMMatToMat(const XMMATRIX& xmMat) {
	MATRIX buf;
	for (int a = 0; a < 4; a++)for (int b = 0; b < 4; b++)
		buf.m[a][b] = xmMat.r[a].m128_f32[b];
	return buf;
}
inline MATRIX& MGetScale(float x, float y, float z) {
	MATRIX mat = {
		{
			{x, 0.f, 0.f, 0.f},
			{0.f, y, 0.f, 0.f},
			{0.f, 0.f, z, 0.f},
			{0.f, 0.f, 0.f, 1.f},
		},
	};

	return mat;
}
inline MATRIX& MGetScale(VECTOR scale) {
	return MGetScale(scale.x, scale.y, scale.z);
}
inline MATRIX& MGetTranslate(float x, float y, float z) {
	MATRIX mat = {
		{
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ x, y, z, 1.f },
		},
	};

	return mat;
}
inline MATRIX& MGetTranslate(VECTOR trans) {
	return MGetTranslate(trans.x, trans.y, trans.z);
}
inline MATRIX& MGetRotX(float axis) {
	float Sin = sin(axis);
	float Cos = cos(axis);
	MATRIX m;
	m.m[0][0] = 1.f; m.m[0][1] = 0.f;  m.m[0][2] = 0.f; m.m[0][3] = 0.f;
	m.m[1][0] = 0.f; m.m[1][1] = Cos;  m.m[1][2] = Sin; m.m[1][3] = 0.f;
	m.m[2][0] = 0.f; m.m[2][1] = -Sin; m.m[2][2] = Cos; m.m[2][3] = 0.f;
	m.m[3][0] = 0.f; m.m[3][1] = 0.f;  m.m[3][2] = 0.f; m.m[3][3] = 1.f;
	return m;
}
inline MATRIX& MGetRotY(float axis) {
	float Sin = sin(axis);
	float Cos = cos(axis);
	MATRIX m;
	m.m[0][0] = Cos; m.m[0][1] = 0.f;  m.m[0][2] = -Sin; m.m[0][3] = 0.f;
	m.m[1][0] = 0.f; m.m[1][1] = 1.f;  m.m[1][2] = 0.f; m.m[1][3] = 0.f;
	m.m[2][0] = Sin; m.m[2][1] = 0.f;  m.m[2][2] = Cos; m.m[2][3] = 0.f;
	m.m[3][0] = 0.f; m.m[3][1] = 0.f;  m.m[3][2] = 0.f; m.m[3][3] = 1.f;
	return m;
}
inline MATRIX& MGetRotZ(float axis) {
	float Sin = sin(axis);
	float Cos = cos(axis);
	MATRIX m;
	m.m[0][0] = Cos;  m.m[0][1] = Sin; m.m[0][2] = 0.f; m.m[0][3] = 0.f;
	m.m[1][0] = -Sin; m.m[1][1] = Cos; m.m[1][2] = 0.f; m.m[1][3] = 0.f;
	m.m[2][0] = 0.f;  m.m[2][1] = 0.f; m.m[2][2] = 1.f; m.m[2][3] = 0.f;
	m.m[3][0] = 0.f;  m.m[3][1] = 0.f; m.m[3][2] = 0.f; m.m[3][3] = 1.f;
	return m;
}
inline VECTOR VTransform(VECTOR vect, MATRIX mat) {
	VECTOR buf;
	buf.x = vect.x * mat.m[0][0] + vect.y * mat.m[1][0] + vect.z * mat.m[2][0] + mat.m[3][0];
	buf.y = vect.x * mat.m[0][1] + vect.y * mat.m[1][1] + vect.z * mat.m[2][1] + mat.m[3][1];
	buf.z = vect.x * mat.m[0][2] + vect.y * mat.m[1][2] + vect.z * mat.m[2][2] + mat.m[3][2];

	buf.x = vect.x * mat.m[0][0] + vect.y * mat.m[0][1] + vect.z * mat.m[0][2];
	buf.y = vect.x * mat.m[1][0] + vect.y * mat.m[1][1] + vect.z * mat.m[1][2];
	buf.z = vect.x * mat.m[2][0] + vect.y * mat.m[2][1] + vect.z * mat.m[2][2];
	return buf;
}

struct VERTEX {
	VECTOR pos;
	VECTOR norm;
	VECTOR binorm;
	VECTOR tangent;
	VECTOR2D uv;
};
inline D3D11_INPUT_ELEMENT_DESC* GetVertexIED(void) {
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	return ied;
}
struct VERTEX2D {
	VECTOR pos;
	VECTOR2D uv;
};

struct POLYGON {
	VERTEX vert[3];
};

struct CONSTANT_BUFFER {
	ID3D11Buffer* buffer;
	UINT bufferSize;
};

struct CONSTANT_BUFFER_DIRECTIONALLIGHT {
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4   lightDirection; // [3]‚Éƒ‰ƒCƒg‚Ì‹­‚³
	XMFLOAT4   lightColor;
	XMFLOAT4   eyeDirection;
};

struct CONSTANT_BUFFER_POINTLIGHT {
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4   lightPos;
	XMFLOAT4   lightPal;	// x:range yzw:atten
};

enum FILE_EXTENSION {
	FILE_EXTENSION_UNKNOWN,
	FILE_EXTENSION_FBX,
	FILE_EXTENSION_FSM	// FSLibModel
};

void CalcTangentAndBinormal(
	VECTOR* p0, VECTOR2D* uv0,
	VECTOR* p1, VECTOR2D* uv1,
	VECTOR* p2, VECTOR2D* uv2,
	VECTOR* outTangent, VECTOR* outBinormal);