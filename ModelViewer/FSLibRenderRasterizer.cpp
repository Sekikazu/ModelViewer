#include "FSLibRenderRasterizer.h"

RENDER_RASTERIZER::RENDER_RASTERIZER(void) {
	
}

RENDER_RASTERIZER::~RENDER_RASTERIZER(void) {
	RELEASE(DefaultRS);
	RELEASE(CullOffRS);
	RELEASE(DepthOffRS);
}

void RENDER_RASTERIZER::Init(void) {
	extern ID3D11Device* Device;

	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	
	// Default
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = TRUE;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	Device->CreateRasterizerState(&RasterizerDesc, &DefaultRS);

	// WireFrame
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	Device->CreateRasterizerState(&RasterizerDesc, &WireFrameRS);
	

	// CullOff
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = TRUE;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	Device->CreateRasterizerState(&RasterizerDesc, &CullOffRS);

	// DepthOff
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	RasterizerDesc.DepthClipEnable = FALSE;
	RasterizerDesc.MultisampleEnable = TRUE;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	Device->CreateRasterizerState(&RasterizerDesc, &DepthOffRS);
}

void RENDER_RASTERIZER::SetDefault(void) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->RSSetState(DefaultRS);
}

void RENDER_RASTERIZER::SetWireFrame(void) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->RSSetState(WireFrameRS);
}

void RENDER_RASTERIZER::SetCullOff(void) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->RSSetState(CullOffRS);
}

void RENDER_RASTERIZER::SetDepthOff(void) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->RSSetState(DepthOffRS);
}
