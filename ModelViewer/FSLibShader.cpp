#include "FSLibShader.h"

VERTEX_SHADER::VERTEX_SHADER(void) {
	inputLayout = nullptr;
	vertexShader = nullptr;
}

VERTEX_SHADER::VERTEX_SHADER(const BYTE* byteCode, size_t codeSize, D3D11_INPUT_ELEMENT_DESC* ied, size_t iedSize) {
	extern ID3D11Device* Device;

	// InputLayout
	inputLayout = nullptr;
	Device->CreateInputLayout(ied, iedSize, byteCode, codeSize, &inputLayout);

	// VertexShader
	vertexShader = nullptr;
	Device->CreateVertexShader(byteCode, codeSize, NULL, &vertexShader);
}

VERTEX_SHADER::~VERTEX_SHADER(void) {
	/*if (inputLayout != nullptr)inputLayout->Release();
	inputLayout = nullptr;
	if (vertexShader != nullptr)vertexShader->Release();
	vertexShader = nullptr;*/
}


PIXEL_SHADER::PIXEL_SHADER(void) {
	pixelShader = nullptr;
}

PIXEL_SHADER::PIXEL_SHADER(const BYTE* byteCode, size_t codeSize) {
	extern ID3D11Device* Device;

	pixelShader = nullptr;
	HRESULT hr = Device->CreatePixelShader(byteCode, codeSize, NULL, &pixelShader);
}

PIXEL_SHADER::~PIXEL_SHADER(void) {
	//if (pixelShader != nullptr)pixelShader->Release();
}