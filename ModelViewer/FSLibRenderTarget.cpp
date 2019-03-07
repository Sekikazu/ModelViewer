#include "FSLibRenderTarget.h"

RENDER_TARGET::RENDER_TARGET(void) {
	width = height = 0;
	renderTargetView = nullptr;
	depthStencilView = nullptr;
	depthStencilTexture = nullptr;
}

RENDER_TARGET::RENDER_TARGET(long targetWidth, long targetHeight) {
	extern ID3D11Device* Device;
	extern ID3D11DeviceContext* DeviceContext;
	extern WINDOW_CONTROLLER wndCont;
	extern IDXGISwapChain* SwapChain;

	width = targetWidth;
	height = targetHeight;

	// 深度ステンシル用テクスチャリソースを作成
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = width;
	txDesc.Height = height;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	if (FAILED(Device->CreateTexture2D(&txDesc, NULL, &depthStencilTexture))) {
		depthStencilTexture = nullptr;
	}

	// 深度ステンシルビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	if (FAILED(Device->CreateDepthStencilView(depthStencilTexture, &dsDesc, &depthStencilView))) {
		depthStencilView = nullptr;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D *BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();
	DeviceContext->ClearRenderTargetView(renderTargetView, wndCont.GetBackColor().col);
	DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

RENDER_TARGET::~RENDER_TARGET(void) {
	renderTargetView->Release();
	depthStencilView->Release();
	depthStencilTexture->Release();
}

void RENDER_TARGET::ClearScreen(void) {
	extern ID3D11DeviceContext* DeviceContext;
	extern WINDOW_CONTROLLER wndCont;

	DeviceContext->ClearRenderTargetView(renderTargetView, wndCont.GetBackColor().col);
	DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void RENDER_TARGET::Set(void) {
	extern ID3D11DeviceContext* DeviceContext;
	DeviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	ID3D11DepthStencilState* depthStencilState;
}
