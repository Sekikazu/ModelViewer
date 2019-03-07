#pragma once
#include "FSLibStructure.h"
#include "WindowController.h"

class RENDER_TARGET {
private:
	long width, height;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilTexture;

public:
	RENDER_TARGET(void);
	RENDER_TARGET(long targetWidth, long targetHeight);

	~RENDER_TARGET(void);

	void ClearScreen(void);
	void Set(void);
};
