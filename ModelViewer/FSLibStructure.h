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
	float pal[2];

	float operator[](int index) {
		return (float)pal[index];
	}
	VECTOR2D operator+(const VECTOR2D& add) {
		VECTOR2D buf;
		buf.pal[0] = pal[0] + add.pal[0];
		buf.pal[1] = pal[1] + add.pal[1];
		return buf;
	}
	VECTOR2D operator-(const VECTOR2D& sub) {
		VECTOR2D buf;
		buf.pal[0] = pal[0] - sub.pal[0];
		buf.pal[1] = pal[1] - sub.pal[1];
		return buf;
	}
	VECTOR2D operator*(const float& scalar) {
		pal[0] *= scalar;
		pal[1] *= scalar;
		return *this;
	}
	float operator*(const VECTOR2D& dot) {
		float buf;
		buf = pal[0] * dot.pal[0] + pal[1] * dot.pal[1];
		return buf;
	}
	VECTOR2D& operator+=(const VECTOR2D& add) {
		pal[0] += add.pal[0];
		pal[1] += add.pal[1];
		return *this;
	}
	VECTOR2D& operator-=(const VECTOR2D& sub) {
		pal[0] -= sub.pal[0];
		pal[1] -= sub.pal[1];
		return *this;
	}
	VECTOR2D& operator*=(const float& scalar) {
		pal[0] *= scalar;
		pal[1] *= scalar;
		return *this;
	}
	float operator*=(const VECTOR2D& dot) {
		float buf;
		buf = pal[0] * dot.pal[0] + pal[1] * dot.pal[1];
		return buf;
	}
};
inline VECTOR2D VGet2D(float x, float y) {
	VECTOR2D buf;
	buf.pal[0] = x; buf.pal[1] = y;
	return buf;
}

class VECTOR {
private:
	float pal[3];

public:
	float& operator[](int index) {
		return (float)pal[index];
	}
	VECTOR operator+(const VECTOR& add) {
		VECTOR buf;
		buf.pal[0] = pal[0] + add.pal[0];
		buf.pal[1] = pal[1] + add.pal[1];
		buf.pal[2] = pal[2] + add.pal[2];
		return buf;
	}
	VECTOR operator-(const VECTOR& sub) {
		VECTOR buf;
		buf.pal[0] = pal[0] - sub.pal[0];
		buf.pal[1] = pal[1] - sub.pal[1];
		buf.pal[2] = pal[2] - sub.pal[2];
		return buf;
	}
	VECTOR operator*(const float& scalar) {
		pal[0] *= scalar;
		pal[1] *= scalar;
		pal[2] *= scalar;
		return *this;
	}
	float operator*(const VECTOR& dot) {
		float buf;
		buf = pal[0] * dot.pal[0] + pal[1] * dot.pal[1] + pal[2] * dot.pal[2];
		return buf;
	}
	VECTOR& operator+=(const VECTOR& add) {
		pal[0] += add.pal[0];
		pal[1] += add.pal[1];
		pal[2] += add.pal[2];
		return *this;
	}
	VECTOR& operator-=(const VECTOR& sub) {
		pal[0] -= sub.pal[0];
		pal[1] -= sub.pal[1];
		pal[2] -= sub.pal[2];
		return *this;
	}
	VECTOR& operator*=(const float& scalar) {
		pal[0] *= scalar;
		pal[1] *= scalar;
		pal[2] *= scalar;
		return *this;
	}
	float operator*=(const VECTOR& dot) {
		float buf;
		buf = pal[0] * dot.pal[0] + pal[1] * dot.pal[1] + pal[2] * dot.pal[2];
		return buf;
	}
	operator XMVECTOR() {
		XMVECTOR xmv;
		xmv.m128_f32[0] = pal[0];
		xmv.m128_f32[1] = pal[1];
		xmv.m128_f32[2] = pal[2];
		xmv.m128_f32[3] = 0;
		return xmv;
	}

	inline VECTOR(){
		pal[0] = 0.f;
		pal[1] = 0.f;
		pal[2] = 0.f;
	}
	inline VECTOR(const VECTOR &vect) {
		pal[0] = vect.pal[0];
		pal[1] = vect.pal[1];
		pal[2] = vect.pal[2];
	}
	inline VECTOR(const float x, const float y, const float z) {
		pal[0] = x;
		pal[1] = y;
		pal[2] = z;
	}
};
inline VECTOR VGet(float x, float y, float z) {
	VECTOR buf;
	buf[0] = x; buf[1] = y; buf[2] = z;
	return buf;
}
inline VECTOR VGet(XMVECTOR xmV) {
	VECTOR buf = VGet(xmV.m128_f32[0], xmV.m128_f32[1], xmV.m128_f32[2]);
	return buf;
}
inline VECTOR VNorm(VECTOR vect) {
	float size = sqrtf(vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2]);
	vect[0] /= size;
	vect[1] /= size;
	vect[2] /= size;
	return vect;
}
inline VECTOR VNorm(float x, float y, float z) {
	VECTOR buf;
	float size = sqrtf(x * x + y * y + z * z);
	buf[0] = x / size; buf[1] = y / size; buf[2] = z / size;
	return buf;
}
inline VECTOR VCross(VECTOR a, VECTOR b) {
	VECTOR buf;
	buf[0] = a[1]*b[2] - a[2]*b[1];
	buf[1] = a[2]*b[0] - a[0]*b[2];
	buf[2] = a[0]*b[1] - a[1]*b[0];
	return buf;
}
inline D3D11_INPUT_ELEMENT_DESC  GetVectorIED(void) {
	D3D11_INPUT_ELEMENT_DESC ied = { "VECTOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	return ied;
}

struct VECTOR4D {
	float x, y, z, w;
};
inline VECTOR4D VGet4D(float x, float y, float z, float w) {
	VECTOR4D buf;
	buf.x = x;
	buf.y = y;
	buf.z = z;
	buf.w = w;
	return buf;
}
inline VECTOR4D VGet4D(FbxVector4 vect) {
	VECTOR4D buf;
	buf.x = vect[0];
	buf.y = vect[1];
	buf.z = vect[2];
	buf.w = vect[3];
	return buf;
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
	return MGetScale(scale[0], scale[1], scale[2]);
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
	return MGetTranslate(trans[0], trans[1], trans[2]);
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
	buf[0] = vect[0] * mat.m[0][0] + vect[1] * mat.m[1][0] + vect[2] * mat.m[2][0] + mat.m[3][0];
	buf[1] = vect[0] * mat.m[0][1] + vect[1] * mat.m[1][1] + vect[2] * mat.m[2][1] + mat.m[3][1];
	buf[2] = vect[0] * mat.m[0][2] + vect[1] * mat.m[1][2] + vect[2] * mat.m[2][2] + mat.m[3][2];

	buf[0] = vect[0] * mat.m[0][0] + vect[1] * mat.m[0][1] + vect[2] * mat.m[0][2];
	buf[1] = vect[0] * mat.m[1][0] + vect[1] * mat.m[1][1] + vect[2] * mat.m[1][2];
	buf[2] = vect[0] * mat.m[2][0] + vect[1] * mat.m[2][1] + vect[2] * mat.m[2][2];
	return buf;
}

struct VERTEX {
	VECTOR pos;
	VECTOR norm;
	VECTOR binorm;
	VECTOR tangent;
	VECTOR2D uv;
	unsigned char born[4];
	float weight;
};
inline D3D11_INPUT_ELEMENT_DESC* GetVertexIED(void) {
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BORN",	  0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },	// MAXボーン数
		{ "WEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	return ied;
}
struct VERTEX_TF {
	VECTOR pos;
	VECTOR norm;
	VECTOR binorm;
	VECTOR tangent;
	VECTOR2D uv;
};
inline D3D11_INPUT_ELEMENT_DESC* GetVertexTFIED(void) {
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
	XMFLOAT4   lightDirection; // [3]にライトの強さ
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