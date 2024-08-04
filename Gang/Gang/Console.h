#pragma once
#include <Windows.h>
void FullScreen();
void GotoxyDouble(int _x, int _y);
void Gotoxy(int _x, int _y);
BOOL GotoPos(int _x, int _y);
COORD CursorPos();
void CursorVis(bool _vis, DWORD _size);
void SetColor(int _textColor = 15, int _bgColor = 0);
int GetColor();
void LockResize();
COORD GetConsoleResolution();
void SetFontSize(UINT _weight, UINT _fontx, UINT _fonty);
enum class COLOR
{
	BLACK = 0, BLUE, GREEN, SKYBLUE, RED,
	VIOLET, YELLOW, LIGHT_GRAY, GRAY, LIGHT_BLUE,
	LIGHT_GREEN, MINT, LIGHT_RED, LIGHT_VIOLET,
	LIGHT_YELLOW, WHITE, END
};
#include<iostream>
using std::cout;
using std::wcout;
using std::endl;