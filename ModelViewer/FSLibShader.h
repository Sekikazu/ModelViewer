#pragma once
#include "FSLibStructure.h"

class VERTEX_SHADER {
private:
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;

public:
	VERTEX_SHADER(void);
	VERTEX_SHADER(const BYTE* byteCode, size_t codeSize, D3D11_INPUT_ELEMENT_DESC*ied, size_t iedSize);
	~VERTEX_SHADER(void);
	ID3D11InputLayout* GetInputLayout(void) { return inputLayout; }
	ID3D11VertexShader* Get(void) { return vertexShader; }
};

class PIXEL_SHADER {
private:
	ID3D11PixelShader* pixelShader;

public:
	PIXEL_SHADER(void);
	PIXEL_SHADER(const BYTE* byteCode, size_t codeSize);
	~PIXEL_SHADER(void);
	ID3D11PixelShader* Get(void) { return pixelShader; }
};
