#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <Windows.h>

HRESULT InitKeyboard();
void UninitKeyboard();
void UpdateKeyboard();
void DrawKeyboard();

void InstantForceInput(int nKey, bool bPress);

bool IsKeyPress(int nKey);
bool IsKeyTrigger(int nKey);
bool IsKeyRelease(int nKey);
bool IsKeyRepeat(int nKey);


#endif // __KEYBOARD_H__