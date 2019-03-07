#pragma once
#include "FSLibStructure.h"
#include "FSLibModel.h"

class SKY_COLOR {
private:
	MODEL model;
	VERTEX_SHADER vertexShader;
	PIXEL_SHADER pixelShader;

public:
	SKY_COLOR(void);
	void Init(void);
	void Draw(void);
};
