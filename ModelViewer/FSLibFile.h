#pragma once
#include "FSLibStructure.h"

class FILE_MANAGER {
private:
	FILE* fp;
	FILE_EXTENSION type;

public:
	FILE_MANAGER(char* fileName, char* mode);
	~FILE_MANAGER(void);
	FILE* Get(void);
	FILE_EXTENSION Type(void);
};