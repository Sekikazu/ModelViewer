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

bool ProcessMessage(void);	/* ���b�Z�[�W����  �����[�v�Ăяo�� */
bool ScreenFlip(void);		/* �^�[�Q�b�g�r���[��\��ʂɓ]�� */	
bool FSLibFinalize(void);

CAMERA* GetCamera(void);
LIGHT* GetLight(void);
CONSTANT_MANAGER* GetConstantManager(void);
KEYBOARD* GetKeyboard(void);
FONT_MANAGER* GetFontManager(void);
FRAME_RATE* GetFrameRateManager(void);
RENDER_INFO* GetRenderInfo(void);
