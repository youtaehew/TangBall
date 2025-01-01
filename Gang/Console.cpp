#include <Windows.h>
#include "console.h"

void FullScreen()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void GotoxyDouble(int _x, int _y)
{
	//콘솔창 핸들
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//커서관련 구조체

	COORD Cur = { _x * 2, _y }; //{_x*2, _y}가 더 자연스러울수 있음
	//커서위치 설정 함수
	SetConsoleCursorPosition(hOut, Cur);
}

void Gotoxy(int _x, int _y)
{
	//콘솔창 핸들
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//커서관련 구조체

	COORD Cur = { _x, _y }; //{_x*2, _y}가 더 자연스러울수 있음
	//커서위치 설정 함수
	SetConsoleCursorPosition(hOut, Cur);
}

BOOL GotoPos(int _x, int _y)
{
	//콘솔창 핸들
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//커서관련 구조체

	COORD Cur = { _x * 2, _y }; //{_x*2, _y}가 더 자연스러울수 있음
	//커서위치 설정 함수
	return SetConsoleCursorPosition(hOut, Cur);
}

COORD CursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE)
		, &buf);
	return buf.dwCursorPosition;
}

void CursorVis(bool _vis, DWORD _size)
{
	CONSOLE_CURSOR_INFO curinfo;
	curinfo.dwSize = _size; // 커서굵기
	curinfo.bVisible = _vis; // on off 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE)
		, &curinfo);
}

void SetColor(int _textColor, int _bgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE)
		, (_bgColor << 4) | _textColor);
}

int GetColor()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE)
		, &info);
	int color = info.wAttributes & 0xf;
	return color;
}

void LockResize()
{
	HWND console = GetConsoleWindow();
	if (nullptr != console)
	{
		LONG style = GetWindowLong(console, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL;// &~WS_CAPTION;

		SetWindowLong(console, GWL_STYLE, style);
	}
}

COORD GetConsoleResolution()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE)
		, &info);

	short width = info.srWindow.Right - info.srWindow.Left + 1;
	short height = info.srWindow.Bottom - info.srWindow.Top + 1;

	return COORD{ width, height };
}

void SetFontSize(UINT _weight, UINT _fontx, UINT _fonty)
{
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

	GetCurrentConsoleFontEx(hout, false, &font);
	font.FontWeight = _weight;
	font.dwFontSize.X = _fontx;
	font.dwFontSize.Y = _fonty;
	GetCurrentConsoleFontEx(hout, false, &font);
}

