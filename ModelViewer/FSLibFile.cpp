#include "FSLibFile.h"

FILE_MANAGER::FILE_MANAGER(char* name, char* mode) {
	vector<char> extension(0);

	int i = 0;
	for (i = 0; i < 256; i++) {
		if (name[i] == '.') {
			int a = 0;
			do {
				++a;

				extension.push_back(name[i + a]);
			} while (name[i + a] != '\0');
			break;
		}

		if (name[i] == '\0') {
			type = FILE_EXTENSION_UNKNOWN;
			return;
		}
	}

	string s = &extension[0];
	if (s == "fbx")type = FILE_EXTENSION_FBX;
	else if (s == "fsm")type = FILE_EXTENSION_FSM;
	else type = FILE_EXTENSION_UNKNOWN;

	// Open
	fopen_s(&fp, name, mode);
}

FILE_MANAGER::~FILE_MANAGER(void) {
	fclose(fp);
}

FILE* FILE_MANAGER::Get(void) {
	return fp;
}

FILE_EXTENSION FILE_MANAGER::Type(void) {
	return type;
}