#pragma once
#include "FSLibStructure.h"

class KEYBOARD {
private:
	unique_ptr<Keyboard> ptr;
	Keyboard::State releaseBuf;
	Keyboard::State pushBuf;
	/*
	unsigned int
		back, tab, retn, lShift, rShift, lCont, rCont, escp, space, pgup, pgdn, end, home, 
		left, up, right, down, insert, delt, minus, yen, prevtrack, period, slash, lAlt, rAlt,
		scroll, semicolon, colon, lbracket, rbracket, at, backslash, commma, kanji, convert, noconvert,
		kana, apps, capslock, sysrq, pause, lwin, rwin, numlock, numPad[10];

	void KeyUpdate(unsigned int* key, bool flag);
	*/

public:
	KEYBOARD(void);
	~KEYBOARD(void);

	//void Update(void);
	void Reset(void);

	Keyboard::State& State(void);
};