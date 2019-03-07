#pragma once
#include "FSLibStructure.h"


class RENDER_RASTERIZER {
private:
	ID3D11RasterizerState* DefaultRS;
	ID3D11RasterizerState* WireFrameRS;
	ID3D11RasterizerState* CullOffRS;
	ID3D11RasterizerState* DepthOffRS;

public:
	RENDER_RASTERIZER(void);
	~RENDER_RASTERIZER(void);
	void Init(void);

	void SetDefault(void);
	void SetWireFrame(void);
	void SetCullOff(void);
	void SetDepthOff(void);
};
