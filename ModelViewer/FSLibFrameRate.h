#pragma once
#include "FSLibStructure.h"
#include <mmsystem.h>

class FRAME_RATE {
private:
	float minFrameTime;
	float frameTime;
	float calculateRatio;
	float fps;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;

public:
	FRAME_RATE(void);
	void SetFrameTime(float time = 1.f / 60.f);
	void Anchor(void);
	float GetFrameTime(void);	// ƒ~ƒŠ•b’PˆÊ
	float GetCalculateRatio(void);
};
