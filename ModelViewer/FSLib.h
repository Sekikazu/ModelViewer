#pragma once
#include "FSLibStructure.h"
#include "WindowController.h"
#include "FSLibFrameRate.h"
#include "FSLibShader.h"
#include "FSLibRenderTarget.h"
#include "FSLibCamera.h"
#include "FSLibLight.h"
#include "FSLibModel.h"
#include "FSLibConstantBuffer.h"
#include "RenderInfo.h"
#include "FSLibKeyboard.h"
#include "FSLibFont.h"
#include "FSLibTexture.h"
#include "FSLibSkyColor.h"

bool FSLibInitialize(WINDOW_CONTROLLER const&);

bool ProcessMessage(void);	/* メッセージ処理  毎ループ呼び出し */
bool ScreenFlip(void);		/* ターゲットビューを表画面に転写 */	
bool FSLibFinalize(void);

CAMERA* GetCamera(void);
LIGHT* GetLight(void);
CONSTANT_MANAGER* GetConstantManager(void);
KEYBOARD* GetKeyboard(void);
FONT_MANAGER* GetFontManager(void);
FRAME_RATE* GetFrameRateManager(void);
RENDER_INFO* GetRenderInfo(void);
