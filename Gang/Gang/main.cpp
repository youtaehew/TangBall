#include <Windows.h>
#include <fstream>
#include <string>
#include "GameMenu.h"
#include "GameLogic.h"
#include "TitleScene.h"
#include "console.h"

int main() {
    system("title TaengTaengBall | mode con cols=41 lines=25");
    CursorVis(false, 1);
    if (!Title()) {
        return 0;
    }

    GameInit();
    MenuInit();

    while (true) {
        if (!IsStart) {
            MenuUpddate();
        }
        else {
            Update(copyMap, &tPlayer);
            Gotoxy(0, 0);
            Render(copyMap, &tPlayer);
            FrameSync(10);
        }
    }
}
