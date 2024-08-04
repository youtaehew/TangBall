#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>

typedef struct _tagpos
{
    int x;
    int y;
    bool operator ==(const _tagpos& _pos)
    {
        return x == _pos.x && y == _pos.y;
    }
} POS, * PPOS;

typedef struct _tagplayer
{
    POS tPos; // 위치
    bool beforeIsBlock; // 벽밀기
    int jumpHeight;
    int currentJump;
} PLAYER, * PPLAYER;

enum class OBJ_TYPE
{
    AIR = '0', BLOCK, STAR, SPINE, JUMP, ICE
};

const int MAP_WIDTH = 21;
const int MAP_HEIGHT = 20;

extern char copyMap[MAP_HEIGHT][MAP_WIDTH];
extern PLAYER tPlayer;
extern int stage;

void GuideText();
void Render(char _arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER _pPlayer);
void Restart(char copyMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player);
bool Moveable(char arrMap[MAP_HEIGHT][MAP_WIDTH], int y, int x, POS before);
void CheckStar(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player);
void Update(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player);
void MenuFrameSync(unsigned int _Framerate);
void GameInit();
void FrameSync(unsigned int _Framerate);
void MapSetting(int mapNumber);

#endif // GAMELOGIC_H
