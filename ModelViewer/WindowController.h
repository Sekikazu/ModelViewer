#pragma once
#include <string>
#include "FSLibStructure.h"
using namespace std;

class WINDOW_CONTROLLER {
	string windowName;
	COLOR backColor;
	long width, height;

public:
	WINDOW_CONTROLLER(string name, COLOR back, long w, long h);

	const char* GetTitle(void);

	COLOR GetBackColor(void);

	long GetWidth(void);
	long GetHeight(void);
};