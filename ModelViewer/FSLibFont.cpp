//#include <locale.h>
#include "FSLibFont.h"

FONT_MANAGER::FONT_MANAGER(void) {
	spriteBatch = nullptr;
	state = nullptr;
	depthStencilState = nullptr;
}

FONT_MANAGER::~FONT_MANAGER(void) {

}

void FONT_MANAGER::Init(void) {
	extern ID3D11Device* Device;
	extern ID3D11DeviceContext* DeviceContext;

	state.reset(new CommonStates(Device));
	spriteBatch.reset(new SpriteBatch(DeviceContext));

	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

	Device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
}

void FONT_MANAGER::Load(const wchar_t* fontName, SpriteFont** pointer) {
	extern ID3D11Device* Device;

	*pointer = (new SpriteFont(Device, fontName));
}

void FONT_MANAGER::Set(SpriteFont* font) {
	nowFont = font;
}

void FONT_MANAGER::StoreStart(void) {
	spriteBatch->Begin();
}

void FONT_MANAGER::StoreEnd(void) {
	extern ID3D11DeviceContext* DeviceContext;

	spriteBatch->End();
	DeviceContext->OMSetDepthStencilState(depthStencilState, 0);
}

void FONT_MANAGER::Store(VECTOR2D pos, char* string, COLOR col, VECTOR2D origin, float rot, float scale) {
	size_t charSize = 0;
	while (string[charSize] != '\0') {
		charSize++;
	}
	charSize++;

	size_t strSize = 0;
	wchar_t wcstr[256];
	mbstowcs_s(&strSize, wcstr, charSize, string, sizeof(wcstr) / sizeof(wchar_t));

	XMVECTOR color;
	color.m128_f32[0] = col.col[0];
	color.m128_f32[1] = col.col[1];
	color.m128_f32[2] = col.col[2];
	color.m128_f32[3] = col.col[3];

	XMFLOAT2 p = XMFLOAT2(pos.x, pos.y);
	XMFLOAT2 ori = XMFLOAT2(origin.x, origin.y);

	nowFont->DrawString(spriteBatch.get(), wcstr, p, color, rot, ori, scale, SpriteEffects_None, 0.5f);
}
