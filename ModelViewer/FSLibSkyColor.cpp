#include "FSLibSkyColor.h"
#include "FSLib.h"

#include "resources\Shader\skycolorVS.h"
#include "resources\Shader\skycolorPS.h"

SKY_COLOR::SKY_COLOR(void) {
	// Shader
	D3D11_INPUT_ELEMENT_DESC ied[5] = {
		GetVertexIED()[0], GetVertexIED()[1], GetVertexIED()[2], GetVertexIED()[3], GetVertexIED()[4] };
	vertexShader = VERTEX_SHADER(g_vs_skycolor, sizeof(g_vs_skycolor), ied, sizeof(ied) / sizeof(ied[0]));
	pixelShader = PIXEL_SHADER(g_ps_skycolor, sizeof(g_ps_skycolor));

	// Model
	{
		VERTEX vert[6];
		ZeroMemory(vert, sizeof(vert));
		// ビュー座標
		vert[0].pos = VGet(0.f, +10.f, 0.f);
		vert[1].pos = VGet(-10.f, 0.f, -10.f);
		vert[2].pos = VGet(-10.f, 0.f, +10.f);
		vert[3].pos = VGet(+10.f, 0.f, +10.f);
		vert[4].pos = VGet(+10.f, 0.f, -10.f);
		vert[5].pos = VGet(0.f, -10.f, 0.f);

		// 法線は不変
		vert[0].norm = VGet(0.f, +1.f, 0.f);
		vert[1].norm = VNorm(-1.f, 0.f, -1.f);
		vert[2].norm = VNorm(-1.f, 0.f, +1.f);
		vert[3].norm = VNorm(+1.f, 0.f, +1.f);
		vert[4].norm = VNorm(+1.f, 0.f, -1.f);
		vert[5].norm = VGet(0.f, -1.f, 0.f);

		WORD index[24] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 1,

			5, 1, 2,
			5, 2, 3,
			5, 3, 4,
			5, 4, 1,
		};

		model = MODEL(vert, 6, index, 24);
	}

	model.SetVertexShader(&vertexShader);
	model.SetPixelShader(&pixelShader);
}

void SKY_COLOR::Init(void) {
	
}

void SKY_COLOR::Draw(void) {
	CONSTANT_MANAGER* constManager = GetConstantManager();
	RENDER_INFO* renderInfo = GetRenderInfo();

	ID3D11Buffer* constBuffer = nullptr;
	size_t constSize = 0;
	constManager->SetConstant_Directional(model.GetWorldMatrix_XM(), 0, &constBuffer, &constSize);
	model.SetConstantBuffer(constBuffer, constSize); // TODO:コンスタントバッファを分離 これをなくす
	renderInfo->Rasterizer()->SetCullOff();
	model.Render();
}