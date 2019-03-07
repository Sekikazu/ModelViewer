#include "WindowController.h"

WINDOW_CONTROLLER::WINDOW_CONTROLLER(string name, COLOR back, long w, long h) {
	windowName = name;
	backColor = back;
	width = w;
	height = h;
}

const char* WINDOW_CONTROLLER::GetTitle(void) {
	const char* c = windowName.c_str();
	return c;
}

COLOR WINDOW_CONTROLLER::GetBackColor(void) { return backColor; }

long WINDOW_CONTROLLER::GetWidth(void) { return width; }
long WINDOW_CONTROLLER::GetHeight(void) { return height; }
