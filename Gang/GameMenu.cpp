#include <Windows.h>
#include <mmsystem.h>
#include <algorithm>
#include "GameMenu.h"
#include "console.h"
#include "GameLogic.h"
#include"GameLogic.h"
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include"PlaySound.h"

MENUPOS arr[9] = {
    {4, 3}, {17, 3}, {30, 3},
    {4, 11}, {17, 11}, {30, 11},
    {4, 19}, {17, 19}, {30, 19}
};

std::wstring number[9] = { L"①", L"②", L"③", L"④", L"⑤", L"⑥", L"⑦", L"⑧", L"⑨" };

MENUPOS CurrentPos = { 3, 3 };
Stage CurrentState;
int ColorNumber = 1;
bool IsStart;

void MenuInit() {
    system("cls");
    IsStart = false;
    LockResize();
    CurrentState = Stage::first;
    CurrentPos = arr[(int)CurrentState];
}

void MenuUpddate() {
    MenuFrameSync(60);
    MenuMoveUpdate();
    MenuMapRender();
}

void MenuMapRender() {
    if (IsStart) {
        system("cls");
    }
    else {

    DrawBox();
    }
}

void MenuMoveUpdate() {
    int x = CurrentPos.x;
    int y = CurrentPos.y;

    if (GetAsyncKeyState(VK_UP) && 0x8000) {
        if (y == MIN_Height) {
            MenuMovePos(6);
        }
        else {
            MenuMovePos(3, false);
        }
        ColorNumber++;
        Sleep(100);
    }

    if (GetAsyncKeyState(VK_DOWN) && 0x8000) {
        if (y == MAX_Height) {
            MenuMovePos(6, false);
        }
        else {
            MenuMovePos(3);
        }
        ColorNumber++;
        Sleep(100);
    }

    if (GetAsyncKeyState(VK_RIGHT) && 0x8000) {
        if (y == MAX_Height && x == MAX_Width) {
            MenuMovePos(8, false);
        }
        else {
            MenuMovePos(1);
        }
        ColorNumber++;
        Sleep(100);
    }

    if (GetAsyncKeyState(VK_LEFT) && 0x8000) {
        if (x == MIN_Width && y == MIN_Height) {
            MenuMovePos(8);
        }
        else {
            MenuMovePos(1, false);
        }
        ColorNumber++;
        Sleep(100);
    }

    if (GetAsyncKeyState(VK_SPACE) && 0x8000) {
        PlaySFX(L"GameStart.mp3");
        MenuManager(true);
    }
}

void MenuFrameSync(unsigned int _Framerate) {
    clock_t oldtime = clock();
    clock_t curtime;

    while (true) {
        curtime = clock();
        if (curtime - oldtime > 1000 / _Framerate) {
            oldtime = curtime;
            break;
        }
    }
}

void MenuMovePos(int data, bool isPlus) {
    PlaySFX(L"Bounce.mp3");
    int number = isPlus ? (int)CurrentState + data : (int)CurrentState - data;
    CurrentState = (Stage)number;
    MapSetting((int)CurrentState);
    CurrentPos = arr[(int)CurrentState];
    GotoPos(CurrentPos.x, CurrentPos.y);
}

void DrawBox() {
    int prevMode = _setmode(_fileno(stdout), _O_U16TEXT);

    for (int i = 0; i < 9; i++) {
        if (i == (int)CurrentState) {
            if (ColorNumber == 5) {
                ColorNumber = 1;
            }
            SetColor(ColorNumber);
        }

        MENUPOS drawPos = arr[i];
        Gotoxy(drawPos.x, drawPos.y);
        wcout << L"◢ ■ ◣";
        Gotoxy(drawPos.x, drawPos.y + 1);
        wcout << L"▉ " << number[i] << L" ▉";
        Gotoxy(drawPos.x, drawPos.y + 2);
        wcout << L"◥ ■ ◤";
        SetColor((int)COLOR::WHITE);
    }

    int curmode = _setmode(_fileno(stdout), prevMode);
}

void MenuManager(bool isStart) {
    IsStart = isStart;
}
