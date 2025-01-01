#ifndef TITLESCENE_H
#define TITLESCENE_H

void TitleRender();
bool Title();

enum class MENU
{
    FIRST, START, QUIT, END
};

MENU MenuRender();
void ClearRender();
enum class KEY
{
    NONE, UP, DOWN, SPACE
};

KEY KeyController();

#endif // TITLESCENE_H
