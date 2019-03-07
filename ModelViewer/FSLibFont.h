#pragma once
#include "FSLibStructure.h"

class FONT_MANAGER {
private:
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<CommonStates> state;
	SpriteFont* nowFont;
	ID3D11DepthStencilState* depthStencilState;

public:
	FONT_MANAGER(void);
	~FONT_MANAGER(void);
	void Init(void);
	void Load(const wchar_t* fontName, SpriteFont** pointer);
	void Set(SpriteFont* font);
	void StoreStart(void);
	void StoreEnd(void);
	void Store(VECTOR2D pos, char* string, COLOR col
		, VECTOR2D origin, float rot = 0.f, float scale = 1.f);
};
