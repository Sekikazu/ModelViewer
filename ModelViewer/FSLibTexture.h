#pragma once
#include "FSLibStructure.h"
#include "RenderInfo.h"
#include "WindowController.h"

class TEXTURE {
private:
	ID3D11Resource* resource;
	ID3D11ShaderResourceView* view;

public:
	TEXTURE(void);
	TEXTURE(const wchar_t* fileName);
	~TEXTURE(void);

	ID3D11Resource* GetResource(void);
	ID3D11ShaderResourceView* GetView(void);

	bool Load(const wchar_t* fileName);
	bool Draw(VECTOR2D min, VECTOR2D max);
};
