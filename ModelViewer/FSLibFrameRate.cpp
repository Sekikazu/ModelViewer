#include "FSLibFrameRate.h"
#pragma comment(lib, "winmm.lib")

FRAME_RATE::FRAME_RATE(void) {
	minFrameTime = 1.f / 60.f;
	fps = 60.f;
	calculateRatio = 0.f;

	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);
}

void FRAME_RATE::SetFrameTime(float time) {
	minFrameTime = time;
	fps = 1.f / time;
}

void FRAME_RATE::Anchor(void) {
	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < minFrameTime) {
		DWORD sleepTime = static_cast<DWORD>((minFrameTime - frameTime) * 1000.f);

		calculateRatio = (calculateRatio * 0.99f) + (frameTime / minFrameTime * 0.01f);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);

		return;
	}

	if (frameTime > 0.f) {
		fps = (fps*0.99f) + (0.01f / frameTime);
	}

	timeStart = timeEnd;
}

float FRAME_RATE::GetFrameTime(void) {
	return fps;
}

float FRAME_RATE::GetCalculateRatio(void) {
	return calculateRatio;
}