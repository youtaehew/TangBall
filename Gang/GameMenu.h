#ifndef GAMEMENU_H
#define GAMEMENU_H

const int MIN_Width = 4;
const int MAX_Width = 30;
const int MIN_Height = 3;
const int MAX_Height = 19;

typedef struct _menuPos
{
	int x;
	int y;
	bool operator == (const _menuPos& _pos) {
		if (x == _pos.x && y == _pos.y)
			return true;
		else
			return false;
	}
} MENUPOS, * MENUPPOS;
extern bool IsStart;

enum class Stage
{
	first,
	second,
	third,
	fourth,
	fifth,
	sixth,
	seventh,
	eighth,
	ninth,
};

void MenuInit();
void MenuMapRender();
void MenuUpddate();
void MenuMoveUpdate();
void MenuFrameSync(unsigned int _Framerate);
void MenuMovePos(int data, bool isPlus = true);
void DrawBox();
void MenuManager(bool isStart);

#endif // GAMEMENU_H
