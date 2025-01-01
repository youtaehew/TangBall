#include<Windows.h>
#include<algorithm>
#include<fstream>
#include <conio.h> 
#include <fcntl.h>
#include <io.h>
#include "GameLogic.h"
#include "console.h"
#include "GameMenu.h"
#include "PlaySound.h"
#include "TitleScene.h"

using namespace std;


POS startPos[9] = { {0, 0} };
char arrMap[9][MAP_HEIGHT][MAP_WIDTH] = {};
int stage;

char copyMap[MAP_HEIGHT][MAP_WIDTH];
PLAYER tPlayer;

void GuideText() {
	Gotoxy(0, MAP_HEIGHT+1);
	cout << "이동 : ←, →\n";
	cout << "다시하기 : R\n";
	cout << "메뉴 : E";
}
void Render(char _arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER _pPlayer) {
	if (!IsStart) {
		system("cls");
		return;
	}
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			if (_pPlayer->tPos.x == j && _pPlayer->tPos.y == i) {
				SetColor((int)COLOR::LIGHT_YELLOW);
				cout << "●";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::BLOCK) {
				SetColor((int)COLOR::GREEN);
				cout << "▨";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::AIR) {
				cout << "  ";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::STAR) {
				SetColor((int)COLOR::YELLOW);
				cout << "★";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::SPINE) {
				SetColor((int)COLOR::GRAY);
				cout << "쓰";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::JUMP) {
				SetColor((int)COLOR::BLUE);
				wcout << L"▲";
			}
			else if (_arrMap[i][j] == (char)OBJ_TYPE::ICE) {
				SetColor((int)COLOR::WHITE);
				wcout << L"▥";
			}
		}
		cout << endl;
	}
	SetColor((int)COLOR::WHITE);
	GuideText();

}

void Restart(char copyMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player) {
	player->tPos = startPos[stage];
	memcpy(copyMap, arrMap[stage], sizeof(arrMap[stage]));
}

bool Moveable(char arrMap[MAP_HEIGHT][MAP_WIDTH], int y, int x, POS before) {
	return arrMap[y][x] == (char)OBJ_TYPE::AIR || arrMap[y][x] == (char)OBJ_TYPE::STAR || (arrMap[y][x] == (char)OBJ_TYPE::SPINE && y > before.y);
}

void CheckStar(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player) {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if (arrMap[i][j] == (char)OBJ_TYPE::STAR) {
				return;
			}
		}
	}
	MenuManager(false);
	ClearRender();
	//clear
	PlaySFX(L"Clear.mp3");
	while (true) {
		if (GetAsyncKeyState(0x45) & 0x8000) {
			break;
		}
	}

	Restart(arrMap, player);
	
}
void Update(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player) {

	if (GetAsyncKeyState(0x52) && 0x52) {
		Restart(arrMap, player);
	}
	if (GetAsyncKeyState(0x45) & 0x8000) {
		MenuManager(false);
		Restart(arrMap, player);
	}

	POS beforePos = player->tPos;

	if (player->currentJump > 2) {//점프 가능하면 하고
		player->tPos.y--;
		player->currentJump--;
	}
	else if (player->currentJump == 2) {//가능한 점프 맨 위면 한번 쉬고
		player->currentJump--;
	}
	else {
		player->tPos.y++;//쭉 떨어진다
		if (arrMap[player->tPos.y][player->tPos.x] == (char)OBJ_TYPE::ICE) {
			player->jumpHeight = 2;
			player->currentJump = 2;
			arrMap[player->tPos.y][player->tPos.x] = (char)OBJ_TYPE::AIR;
			player->tPos.y -= 2;
			PlaySFX(L"Bounce.mp3");
		}
	}

	if (!Moveable(arrMap, player->tPos.y, player->tPos.x, beforePos)) {//지금 있는곳 블록이면
		if (Moveable(arrMap, player->tPos.y - 2, player->tPos.x, beforePos) && beforePos.y < player->tPos.y) {//내 2칸 위는 갈 수 있으면
			if (arrMap[player->tPos.y][player->tPos.x] == (char)OBJ_TYPE::JUMP) {
				player->jumpHeight = 5;
				player->currentJump = 5;
				PlaySFX(L"HighJump.mp3");
				//high jump
			}
			/*else if (arrMap[player->tPos.y][player->tPos.x] == (char)OBJ_TYPE::ICE) {
				player->jumpHeight = 2;
				player->currentJump = 2;
				arrMap[player->tPos.y][player->tPos.x] = (char)OBJ_TYPE::AIR;
			}*/
			else {
				player->jumpHeight = 2;
				player->currentJump = 2;
				PlaySFX(L"Bounce.mp3");
				//bounce
			}
			player->tPos.y -= 2;

		}
		else if (beforePos.y > player->tPos.y) {
			player->tPos.y += 1;
			player->currentJump = 2;
		}
		else
			player->tPos.y -= 1;
		player->beforeIsBlock = true;
	}

	if (GetAsyncKeyState(VK_RIGHT) && 0x8000 && Moveable(arrMap, player->tPos.y, player->tPos.x + 1, beforePos)) {
		player->tPos.x++;
	}
	if (GetAsyncKeyState(VK_LEFT) && 0x8000 && Moveable(arrMap, player->tPos.y, player->tPos.x - 1, beforePos)) {
		player->tPos.x--;
	}

	if (arrMap[player->tPos.y][player->tPos.x] == (char)OBJ_TYPE::STAR) {
		PlaySFX(L"EatStar.mp3");
		//eat star
		arrMap[player->tPos.y][player->tPos.x] = (char)OBJ_TYPE::AIR;
		CheckStar(arrMap, player);
	}
	else if (arrMap[player->tPos.y][player->tPos.x] == (char)OBJ_TYPE::SPINE) {
		PlaySFX(L"Die.mp3");
		Restart(arrMap, player);
	}

}

void GameInit()
{
	PlayBGM(L"BGM.mp3", 1000);
	_wsetlocale(LC_ALL, L"korean");
	//system("title TaengTaengBall | mode con cols=41 lines=25");
	//0
	{
		startPos[0] = { 0, 13 };
		strcpy_s(arrMap[0][0], "00000000000000000000");
		strcpy_s(arrMap[0][1], "00000000000000000000");
		strcpy_s(arrMap[0][2], "00000000000000000000");
		strcpy_s(arrMap[0][3], "00000000000000000000");
		strcpy_s(arrMap[0][4], "00000000000000000000");
		strcpy_s(arrMap[0][5], "00000000000000000000");
		strcpy_s(arrMap[0][6], "00000000000000000000");
		strcpy_s(arrMap[0][7], "00000000000000000000");
		strcpy_s(arrMap[0][8], "00000000000000000000");
		strcpy_s(arrMap[0][9], "00000000000000000000");
		strcpy_s(arrMap[0][10], "00000000000000000000");
		strcpy_s(arrMap[0][11], "00000000000000000000");
		strcpy_s(arrMap[0][12], "00000000000000000000");
		strcpy_s(arrMap[0][13], "00000000000000000000");
		strcpy_s(arrMap[0][14], "00000000000000000002");
		strcpy_s(arrMap[0][15], "11111111111111111111");
		strcpy_s(arrMap[0][16], "00000000000000000000");
		strcpy_s(arrMap[0][17], "00000000000000000000");
		strcpy_s(arrMap[0][18], "00000000000000000000");
		strcpy_s(arrMap[0][19], "00000000000000000000");
	}

	//1
	{
		startPos[1] = { 10, 13 };
		strcpy_s(arrMap[1][0], "00000000000000000000");
		strcpy_s(arrMap[1][1], "00000000000000000000");
		strcpy_s(arrMap[1][2], "00000000000000000000");
		strcpy_s(arrMap[1][3], "00000000000000000000");
		strcpy_s(arrMap[1][4], "00000000000000000000");
		strcpy_s(arrMap[1][5], "00000000000000000000");
		strcpy_s(arrMap[1][6], "00000000000000000000");
		strcpy_s(arrMap[1][7], "00000000000000000000");
		strcpy_s(arrMap[1][8], "00000000000000000000");
		strcpy_s(arrMap[1][9], "00000000000000000000");
		strcpy_s(arrMap[1][10], "22200000000000000000");
		strcpy_s(arrMap[1][11], "20000000000000000000");
		strcpy_s(arrMap[1][12], "20000000000000000000");
		strcpy_s(arrMap[1][13], "20000000000000000000");
		strcpy_s(arrMap[1][14], "20000000000000000002");
		strcpy_s(arrMap[1][15], "45555551111111331331");
		strcpy_s(arrMap[1][16], "00000000000000000000");
		strcpy_s(arrMap[1][17], "00000000000000000000");
		strcpy_s(arrMap[1][18], "00000000000000000000");
		strcpy_s(arrMap[1][19], "33333333333333333333");
	}//2

	//2
	{
		startPos[2] = { 0, 0 };
		strcpy_s(arrMap[2][0],  "00000000000000000002");
		strcpy_s(arrMap[2][1],  "00000000000000000002");
		strcpy_s(arrMap[2][2],  "55555555555555555555");
		strcpy_s(arrMap[2][3],  "03333333333333333333");
		strcpy_s(arrMap[2][4],  "00000000000000000000");
		strcpy_s(arrMap[2][5],  "00000000000000000000");
		strcpy_s(arrMap[2][6],  "00000000000000000000");
		strcpy_s(arrMap[2][7],  "00000000000000000000");
		strcpy_s(arrMap[2][8],  "40000000000000000000");
		strcpy_s(arrMap[2][9],  "00000000000000000000");
		strcpy_s(arrMap[2][10], "00000000000000000000");
		strcpy_s(arrMap[2][11], "00000000000000000000");
		strcpy_s(arrMap[2][12], "00000000000000000000");
		strcpy_s(arrMap[2][13], "00000000000000000000");
		strcpy_s(arrMap[2][14], "00000000000000000000");
		strcpy_s(arrMap[2][15], "00000000000000000000");
		strcpy_s(arrMap[2][16], "00000000000000000000");
		strcpy_s(arrMap[2][17], "00000000000000000000");
		strcpy_s(arrMap[2][18], "00000000000000000002");
		strcpy_s(arrMap[2][19], "33333333333333333333");
	}//3

	//3
	{
		startPos[3] = { 0, 0 };
		strcpy_s(arrMap[3][0],  "00000000000000000000");
		strcpy_s(arrMap[3][1],  "00000000000000000000");
		strcpy_s(arrMap[3][2],  "10010010010010010000");
		strcpy_s(arrMap[3][3],  "33333333333333333330");
		strcpy_s(arrMap[3][4],  "00000000000000000000");
		strcpy_s(arrMap[3][5],  "05555555555555555555");
		strcpy_s(arrMap[3][6],  "33333333330333333333");
		strcpy_s(arrMap[3][7],  "33333333330333333333");
		strcpy_s(arrMap[3][8],  "33333333330333333333");
		strcpy_s(arrMap[3][9],  "00000000000000000000");
		strcpy_s(arrMap[3][10], "00000000000000000000");
		strcpy_s(arrMap[3][11], "00000000000000000000");
		strcpy_s(arrMap[3][12], "00000000000000000000");
		strcpy_s(arrMap[3][13], "00000000000000220000");
		strcpy_s(arrMap[3][14], "00000002000002002000");
		strcpy_s(arrMap[3][15], "00000000200020000200");
		strcpy_s(arrMap[3][16], "00000000020200000020");
		strcpy_s(arrMap[3][17], "00000000002000000002");
		strcpy_s(arrMap[3][18], "00000000004000000002");
		strcpy_s(arrMap[3][19], "33333333333333333333");
	}

	//4
	{
		startPos[4] = { 0, 0 };
		strcpy_s(arrMap[4][0], "00000000000000000000");
		strcpy_s(arrMap[4][1], "00000000000000000000");
		strcpy_s(arrMap[4][2], "00000000000000000000");
		strcpy_s(arrMap[4][3], "00000000000000000000");
		strcpy_s(arrMap[4][4], "00000000000000000000");
		strcpy_s(arrMap[4][5], "00000000000000000000");
		strcpy_s(arrMap[4][6], "55555555555555555555");
		strcpy_s(arrMap[4][7], "33333333333333333303");
		strcpy_s(arrMap[4][8], "20000000000000000000");
		strcpy_s(arrMap[4][9], "55555555555555555555");
		strcpy_s(arrMap[4][10], "20000000000000000000");
		strcpy_s(arrMap[4][11], "02000000000000000000");
		strcpy_s(arrMap[4][12], "00200000000000000000");
		strcpy_s(arrMap[4][13], "00020000000000000000");
		strcpy_s(arrMap[4][14], "00002000000000000000");
		strcpy_s(arrMap[4][15], "00000200000000000000");
		strcpy_s(arrMap[4][16], "00000020000000000002");
		strcpy_s(arrMap[4][17], "00000002000000000000");
		strcpy_s(arrMap[4][18], "10010010210010010010");
		strcpy_s(arrMap[4][19], "33333333333333333333");
	}
	//5
	{
		startPos[5] = { 0, 15 };
		strcpy_s(arrMap[5][0], "00000000000000000000");
		strcpy_s(arrMap[5][1], "00000000000000000000");
		strcpy_s(arrMap[5][2], "00000000000000000000");
		strcpy_s(arrMap[5][3], "00000000000000000000");
		strcpy_s(arrMap[5][4], "00200000000000000000");
		strcpy_s(arrMap[5][5], "00000000000000000000");
		strcpy_s(arrMap[5][6], "00005000000000000000");
		strcpy_s(arrMap[5][7], "00000050000000000000");
		strcpy_s(arrMap[5][8], "00000000500000000000");
		strcpy_s(arrMap[5][9], "00000000005005005000");
		strcpy_s(arrMap[5][10], "00000000000000000000");
		strcpy_s(arrMap[5][11], "00000000000000000000");
		strcpy_s(arrMap[5][12], "00000000000000000400");
		strcpy_s(arrMap[5][13], "00000000000000000000");
		strcpy_s(arrMap[5][14], "00000000001000000000");
		strcpy_s(arrMap[5][15], "00000000010000000040");
		strcpy_s(arrMap[5][16], "00000000100000000000");
		strcpy_s(arrMap[5][17], "00000001000000000200");
		strcpy_s(arrMap[5][18], "00000010000000000004");
		strcpy_s(arrMap[5][19], "11111113333333333311");
	}
	//6
	{
		startPos[6] = { 0, 15 };
		strcpy_s(arrMap[6][0],  "00000001000000010000");
		strcpy_s(arrMap[6][1],  "00000001010000012200");
		strcpy_s(arrMap[6][2],  "00000000010000012020");
		strcpy_s(arrMap[6][3],  "00000001515555512020");
		strcpy_s(arrMap[6][4],  "00000001510000512020");
		strcpy_s(arrMap[6][5],  "00000041510000512020");
		strcpy_s(arrMap[6][6],  "00000101510000514200");
		strcpy_s(arrMap[6][7],  "00010001500000510200");
		strcpy_s(arrMap[6][8],  "01000001555550510200");
		strcpy_s(arrMap[6][9],  "00000001011115010200");
		strcpy_s(arrMap[6][10], "00000001010005510420");
		strcpy_s(arrMap[6][11], "00000001000001510020");
		strcpy_s(arrMap[6][12], "40000001000001510020");
		strcpy_s(arrMap[6][13], "00100001000001510020");
		strcpy_s(arrMap[6][14], "00001001000001510040");
		strcpy_s(arrMap[6][15], "00000001000001510002");
		strcpy_s(arrMap[6][16], "00000041000001510002");
		strcpy_s(arrMap[6][17], "00001001000001510002");
		strcpy_s(arrMap[6][18], "00100001000001500004");
		strcpy_s(arrMap[6][19], "11111111111111111131");
	}

	//7
	{
		startPos[7] = { 0, 15 };
		strcpy_s(arrMap[7][0],  "00000000000000000000");
		strcpy_s(arrMap[7][1],  "00000000000000000000");
		strcpy_s(arrMap[7][2],  "00000000000000000100");
		strcpy_s(arrMap[7][3],  "00000000000004001000");
		strcpy_s(arrMap[7][4],  "00000000000000010000");
		strcpy_s(arrMap[7][5],  "00000000000000100000");
		strcpy_s(arrMap[7][6],  "00000000000001000000");
		strcpy_s(arrMap[7][7],  "00000000000010000000");
		strcpy_s(arrMap[7][8],  "00000000000100000000");
		strcpy_s(arrMap[7][9],  "20000000001000000000");
		strcpy_s(arrMap[7][10], "20000000010000000000");
		strcpy_s(arrMap[7][11], "20000000100000000000");
		strcpy_s(arrMap[7][12], "20000001000000000000");
		strcpy_s(arrMap[7][13], "00000010000000000000");
		strcpy_s(arrMap[7][14], "00000100000000000000");
		strcpy_s(arrMap[7][15], "00001000000000000000");
		strcpy_s(arrMap[7][16], "00010000000000000000");
		strcpy_s(arrMap[7][17], "00120000000000000000");
		strcpy_s(arrMap[7][18], "01055055055055055255");
		strcpy_s(arrMap[7][19], "13333333333333333333");
	}

	//8
	{
		startPos[8] = { 0, 15 };
		strcpy_s(arrMap[8][0],  "00000000000000000000");
		strcpy_s(arrMap[8][1],  "00000000000000000002");
		strcpy_s(arrMap[8][2],  "00000000000000000222");
		strcpy_s(arrMap[8][3],  "00000000022022022052");
		strcpy_s(arrMap[8][4],  "00000002200200205002");
		strcpy_s(arrMap[8][5],  "00000220500500500002");
		strcpy_s(arrMap[8][6],  "00022050000000000002");
		strcpy_s(arrMap[8][7],  "02205000000000000002");
		strcpy_s(arrMap[8][8],  "20500000000000000002");
		strcpy_s(arrMap[8][9],  "50000000000000000002");
		strcpy_s(arrMap[8][10], "00000000000000000002");
		strcpy_s(arrMap[8][11], "00000000000000000000");
		strcpy_s(arrMap[8][12], "00022020220220220220");
		strcpy_s(arrMap[8][13], "40200202002002002002");
		strcpy_s(arrMap[8][14], "00500505005005005005");
		strcpy_s(arrMap[8][15], "00000000000000000020");
		strcpy_s(arrMap[8][16], "02202202202202202220");
		strcpy_s(arrMap[8][17], "00020020020020020020");
		strcpy_s(arrMap[8][18], "10050050050050050040");
		strcpy_s(arrMap[8][19], "33333333333333333333");
	}


	MapSetting(0);
	tPlayer.beforeIsBlock = false;
	tPlayer.jumpHeight = 2;
	tPlayer.currentJump = 2;
}

void FrameSync(unsigned int _Framerate)
{
	clock_t oldtime = clock();
	clock_t curtime;

	while (true)
	{
		curtime = clock();
		if (curtime - oldtime > 1000 / _Framerate)
		{
			oldtime = curtime;
			break;
		}
	}

}

void MapSetting(int mapNumber)
{
	stage = mapNumber;
	memcpy(copyMap, arrMap[stage], sizeof(arrMap[stage]));
	tPlayer.tPos = startPos[stage];
}
