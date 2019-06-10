#pragma once
#include "FSLibStructure.h"

class ANIMATION {
private:
	string name;
	int boneCount;

public:
	ANIMATION(FbxImporter*, string);
};