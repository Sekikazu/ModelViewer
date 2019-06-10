#pragma once
#include "FSLibAnimation.h"

class ANIMATION_MANAGER {
private:
	int stackCount;
	vector<ANIMATION> animations;

public:
	ANIMATION_MANAGER(FbxImporter*);
};