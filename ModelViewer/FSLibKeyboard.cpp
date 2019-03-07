#include "FSLibKeyboard.h"

KEYBOARD::KEYBOARD(void) {
	ptr = make_unique<Keyboard>();
}

KEYBOARD::~KEYBOARD(void) {
	//ptr->~Keyboard(); スマートポインタなので必要なし
}

/*
void KEYBOARD::Update(void) {
	Keyboard::State kb = ptr->GetState();
	
	KeyUpdate(&back, kb.Back);
	KeyUpdate(&tab, kb.Tab);
	KeyUpdate(&retn, kb.Enter);
	KeyUpdate(&lShift, kb.LeftShift);
	KeyUpdate(&rShift, kb.RightShift);
	KeyUpdate(&lCont, kb.LeftControl);
	KeyUpdate(&rCont, kb.RightControl);
	KeyUpdate(&escp, kb.Escape);
	KeyUpdate(&space, kb.Space);
	KeyUpdate(&pgup, kb.PageUp);
	KeyUpdate(&pgdn, kb.PageDown);
	KeyUpdate(&end, kb.End);
	KeyUpdate(&home, kb.Home);
	
	KeyUpdate(&left, kb.Left);
	KeyUpdate(&up, kb.Up);
	KeyUpdate(&right, kb.Right);
	KeyUpdate(&down, kb.Down);
	KeyUpdate(&insert, kb.Insert);
	KeyUpdate(&delt, kb.Delete);
	KeyUpdate(&minus, kb.OemMinus);
	KeyUpdate(&yen, kb.OemBackslash);
	//KeyUpdate(&prevtrack, );
	KeyUpdate(&period, kb.OemPeriod);
	KeyUpdate(&slash, kb.Divide);
	KeyUpdate(&lAlt, kb.LeftAlt);
	KeyUpdate(&rAlt, kb.RightAlt);
	
	KeyUpdate(&scroll, kb.Scroll);
	KeyUpdate(&semicolon, kb.OemSemicolon);
	KeyUpdate(&colon, kb.);
}

void KEYBOARD::KeyUpdate(unsigned int* key, bool flag) {
	if (!flag)
		*key = 0;

	else {
		++*key;
		if (*key == 0)*key = 1;
	}
}
*/

void KEYBOARD::Reset(void) {
	ptr->Reset();
}

Keyboard::State& KEYBOARD:: State(void) {
	return ptr->GetState();
}
