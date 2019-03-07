#pragma once
#include "FSLibStructure.h"
#include "FSLibRenderTarget.h"
#include "FSLibShader.h"
#include "FSLibConstantBuffer.h"
#include "FSLibRenderRasterizer.h"

class RENDER_INFO {
private:
	// Rasterizer
	RENDER_RASTERIZER rasterizer;

	// Sampler
	ID3D11SamplerState* sampler;

	// Texture
	ID3D11Buffer* texVertexBuffer;
	ID3D11Buffer* texIndexBuffer;
	CONSTANT_BUFFER texConstantBuffer;
	VERTEX_SHADER texVertexShader;
	PIXEL_SHADER texPixelShader;

public:
	RENDER_INFO(void);
	~RENDER_INFO(void);
	void Init(void);

	// Rasterizer
	RENDER_RASTERIZER* Rasterizer(void) { return &rasterizer; }

	// RenderTarget


	// Sampler
	bool SetSampler(D3D11_SAMPLER_DESC&);

	// Texture
	ID3D11Buffer* GetTextureVertexBuffer(void) { return texVertexBuffer; }
	ID3D11Buffer* GetTextureIndexBuffer(void) { return texIndexBuffer; }
	CONSTANT_BUFFER GetTextureConstantBuffer(void) { return texConstantBuffer; }
	VERTEX_SHADER GetTextureVertexShader(void) { return texVertexShader; }
	PIXEL_SHADER GetTexturePixelShader(void) { return texPixelShader; }
};
