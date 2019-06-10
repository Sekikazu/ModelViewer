#include <stdlib.h>
#include "FSLib.h"
#include "resources\Shader\directionVS.h"
#include "resources\Shader\directionPS.h"
#include "resources\Shader\pointVS.h"
#include "resources\Shader\pointPS.h"
#include "resources\Shader\textureVS.h"
#include "resources\Shader\texturePS.h"
#include "resources\Shader\wireframeVS.h"
#include "resources\Shader\wireframePS.h"

#include <Inc\ScreenGrab.h>

//メイン関数
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// 初期化処理
	{
		WINDOW_CONTROLLER wc("FSLib", GetColor(0.f, 0.f, 0.f, 1.f), 1024, 768);
		//WINDOW_CONTROLLER wc("FSLib", GetColor(1.f, 1.f, 0.f, 1.f), 1024, 768);
		if (!FSLibInitialize(wc))return 0;
	}

	FRAME_RATE* frameRate = GetFrameRateManager();
	frameRate->SetFrameTime(1.f / 30.f);

	RENDER_TARGET renderTarget(1024, 768);
	renderTarget.Set();

	RENDER_INFO* renderInfo = GetRenderInfo();

	// Shader
	D3D11_INPUT_ELEMENT_DESC ied[5] = {
		GetVertexIED()[0], GetVertexIED()[1], GetVertexIED()[2], GetVertexIED()[3], GetVertexIED()[4] };
	VERTEX_SHADER vertexShaderPoint(g_vs_point, sizeof(g_vs_point), ied, sizeof(ied)/sizeof(ied[0]));
	VERTEX_SHADER vertexShaderDirect(g_vs_direction, sizeof(g_vs_direction), ied, sizeof(ied) / sizeof(ied[0]));
	VERTEX_SHADER vertexShaderWire(g_vs_wireframe, sizeof(g_vs_wireframe), ied, sizeof(ied) / sizeof(ied[0]));

	PIXEL_SHADER pixelShaderPoint(g_ps_point, sizeof(g_ps_point));
	PIXEL_SHADER pixelShaderDirect(g_ps_direction, sizeof(g_ps_direction));
	PIXEL_SHADER pixelShaderWire(g_ps_wireframe, sizeof(g_ps_wireframe));

	// Font
	FONT_MANAGER* fontManager = GetFontManager();
	SpriteFont* fontPointer = nullptr;
	fontManager->Load(L"resources/Font/msgothic.spritefont", &fontPointer);
	fontManager->Set(fontPointer);

	// Keyboard
	KEYBOARD* kb = GetKeyboard();

	// Model
	MODEL model("resources/Model/matBall.fsm");
	TEXTURE baseColor(L"resources/Model/赤錆_basecolor.png");
	TEXTURE normal(L"resources/Model/赤錆_normal.png");
	TEXTURE roughness(L"resources/Model/赤錆_roughness.png");
	TEXTURE metallic(L"resources/Model/赤錆_metallic.png");
	TEXTURE height(L"resources/Model/赤錆_height.png");
	model.SetBaseColorMap(&baseColor);
	model.SetNormalMap(&normal);
	model.SetRoughnessMap(&roughness);
	model.SetMetallicMap(&metallic);
	model.SetHeightMap(&height);

	//model.SetPosition(VGet(0.f, 0.f, 0.f));

	model.SetVertexShader(&vertexShaderDirect);
	model.SetPixelShader(&pixelShaderDirect);

	// 各種描画要素
	LIGHT* light = GetLight();
	CAMERA* camera = GetCamera();
	CONSTANT_MANAGER* constManager = GetConstantManager();
	
	DIRECTIONAL_LIGHT dl;
	dl.direction = VNorm(-2.f, -4.f, 3.f);
	dl.color = GetColor(1.f, 1.f, 1.f, 1.f);
	light->SetDirectional(0, dl);
	light->CreatePoint(VGet(2.f, 1.5f, 1.f), 5.f, 0.01f, 0.1f, 0.1f);
	camera->SetPositionAndTarget_UpVecY(VGet(0.5f, 1.f, -1.5f), VGet(0.0f, 0.2f, 0.f));

	renderInfo->Rasterizer()->SetDefault();

	//TEXTURE tex(L"resources/Picture/Grid.png");
	SKY_COLOR SkyColor;

	// メインループ
	float length = 3.f;
	float camRotX = 0.f, camRotY = 0.f;
	float lightRotX = 0.f;
	float count = 0.f;
	float lightColor[3] = { 0.5f, 0.5f, 0.5f };
	bool pushBuf = false;
	while (ProcessMessage() && ScreenFlip()) {
		renderTarget.ClearScreen();

		// カメラ移動
		{
			// length
			if (kb->State().S && !pushBuf) length *= 1.05f;
			pushBuf = kb->State().S;

			if (kb->State().W && !pushBuf) length /= 1.05f;
			pushBuf = kb->State().W;

			// rotX
			if (kb->State().Up) {
				camRotX += PI / 180.f;
				if (camRotX > PI / 2.f)camRotX = PI / 2.f;
			}

			if (kb->State().Down) {
				camRotX -= PI / 180.f;
				if (camRotX < -PI / 2.f)camRotX = -PI / 2.f;
			}

			// rotY
			if (kb->State().Left) {
				camRotY -= PI / 180.f;
				if (camRotY < PI)camRotY += 2 * PI;
			}

			if (kb->State().Right) {
				camRotY += PI / 180.f;
				if (camRotY > PI)camRotY -= 2 * PI;
			}

			camera->SetPositionAndTarget_UpVecY(VGet(0.0f, 0.5f, 0.f) + VTransform(VTransform(VGet(0.f, 0.0f, -1.f), MGetRotX(camRotX)), MGetRotY(camRotY)) * length, VGet(0.0f, 0.5f, 0.f));
			//camera->SetPositionAndTarget_UpVecY(VGet(1.f, 0.3f, -1.2f) * length, VGet(0.0f, 0.0f, 0.f));
		}

		// ライト移動
		{
			//if (!kb->State().A)count += 3.f*PI/360.f;
			////count = 0.2443f;
			//if (count > 2.f * PI)count -= 2.f * PI;
			//dl.direction = VNorm(0.f, -1.f, 3.f);
			//dl.direction = VTransform(dl.direction, MGetRotY(count));

			// rotX
			float maxLightRotX = PI / 2.2f;
			if (kb->State().Q) {
				lightRotX -= PI / 180.f;
				if (lightRotX < -maxLightRotX)lightRotX = -maxLightRotX;
			}

			if (kb->State().A) {
				lightRotX += PI / 180.f;
				if (lightRotX > maxLightRotX)lightRotX = maxLightRotX;
			}

			// Collor
			/*
			if (kb->State().Z)lightColor[0] += 1.f / 300.f;
			if (lightColor[0] > 1.f)lightColor[0] -= 1.f;
			if (kb->State().X)lightColor[1] += 1.f / 300.f;
			if (lightColor[1] > 1.f)lightColor[1] -= 1.f;
			if (kb->State().C)lightColor[2] += 1.f / 300.f;
			if (lightColor[2] > 1.f)lightColor[2] -= 1.f;
			*/

			float lamda[3] = { 0.7f, 0.5461f, 0.4358f };
			for (int i = 0; i < 3; ++i)lamda[i] = (lamda[i] / lamda[2]) * 0.8f + 0.2f;
			float sin = -sinf(lightRotX);
			float air = 0.015f;
			float lightLen = -sin + sqrtf(sin*sin + 2 * air + air * air);
			float str = 5.f;
			for (int i = 0; i < 3; ++i)
				lightColor[i] = 1.f - str * lightLen / (lamda[i] * lamda[i] * lamda[i] * lamda[i]);

			if (kb->State().Z) lightColor[0] = 0.f;
			if (kb->State().X) lightColor[1] = 0.f;
			if (kb->State().C) lightColor[2] = 0.f;

			dl.direction = VTransform(VGet(0.f, 0.f, 1.f), MGetRotX(lightRotX));
			dl.color = GetColor(lightColor[0], lightColor[1], lightColor[2], 1.f);
			light->SetDirectional(0, dl);
		}

		// Model
		{
			ID3D11Buffer* constBuffer = nullptr;
			size_t constSize = 0;
			constManager->SetConstant_Directional(model.GetWorldMatrix_XM(), 0, &constBuffer, &constSize);
			model.SetConstantBuffer(constBuffer, constSize); // TODO:コンスタントバッファを分離 これをなくす
			renderInfo->Rasterizer()->SetDefault();
			model.Render();
		}

		SkyColor.Draw();

		// Textures
		baseColor.Draw(VGet2D(20.f, 40.f), VGet2D(220.f, 240.f));
		normal.Draw(VGet2D(20.f, 500.f), VGet2D(220.f, 700.f));
		roughness.Draw(VGet2D(804.f, 40.f), VGet2D(1004.f, 240.f));
		height.Draw(VGet2D(804.f, 500.f), VGet2D(1004.f, 700.f));

		//SaveWICTextureToFile()

		// フォント
		{
			char fp[20];
			sprintf_s(fp, "FPS:%3.2f\0", frameRate->GetFrameTime());
			char calRatio[20];
			sprintf_s(calRatio, "計算占有率:%3.2f%%", frameRate->GetCalculateRatio()*100.f);
			char lengthStr[20];
			sprintf_s(lengthStr, "Length:%3.2f", length);
			char lightColorStr[30];
			sprintf_s(lightColorStr, "LiColor:%2.2f,%2.2f,%2.2f", dl.color.col[0], dl.color.col[1], dl.color.col[2]);

			fontManager->StoreStart();
			fontManager->Store(VGet2D(20.f, 15.f), "開発中\0", GetColor(0.8f, 0.f, 0.f, 1.f), VGet2D(10.f, 10.f));
			fontManager->Store(VGet2D(100.f, 15.f), fp, GetColor(1.f, 1.f, 1.f, 1.f), VGet2D(10.f, 10.f));
			fontManager->Store(VGet2D(200.f, 15.f), calRatio, GetColor(1.f, 1.f, 1.f, 1.f), VGet2D(10.f, 10.f));
			fontManager->Store(VGet2D(350.f, 15.f), lengthStr, GetColor(1.f, 1.f, 1.f, 1.f), VGet2D(10.f, 10.f));
			fontManager->Store(VGet2D(500.f, 15.f), lightColorStr, GetColor(1.f, 1.f, 1.f, 1.f), VGet2D(10.f, 10.f));
			fontManager->StoreEnd();
		}
	}

	// 終了処理
	FSLibFinalize();
	return 0;
}
