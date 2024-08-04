#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#include <string>
#include "PlaySound.h"

UINT BGMid;
UINT SFXid;

void PlayBGM(LPCWSTR soundName, int volume)
{
    mciSendCommand(BGMid, MCI_CLOSE, NULL, (DWORD)NULL);
    MCI_OPEN_PARMS openBGM;
    openBGM.lpstrDeviceType = TEXT("mpegvideo");
    openBGM.lpstrElementName = soundName;

    mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openBGM);
    BGMid = openBGM.wDeviceID;

    std::wstring msg = L"setaudio " + std::wstring(soundName) + L" volume to " + std::to_wstring(volume);
    mciSendString(msg.c_str(), NULL, NULL, NULL);

    MCI_PLAY_PARMS playBGM;
    mciSendCommand(BGMid, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&playBGM);

    //mciSendCommand(BGMid, MCI_PAUSE, MCI_NOTIFY, (DWORD_PTR)&playBGM);
    //
    //mciSendCommand(BGMid, MCI_RESUME, NULL, (DWORD_PTR)&playBGM);
    //
    //mciSendCommand(BGMid, MCI_CLOSE, NULL, (DWORD_PTR)&playBGM);
}

void PlaySFX(LPCWSTR soundname)
{
    // PLAY
    MCI_PLAY_PARMS playEffect;
    if (SFXid != 0)
    {
        mciSendCommand(SFXid, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)&playEffect);
        mciSendCommand(SFXid, MCI_CLOSE, NULL, (DWORD)NULL); // RAII
        SFXid = 0;
    }
    // OPEN
    MCI_OPEN_PARMS openEffect;
    // mp3: mpegvideo, wav: waveaudio, avi: avivideo
    openEffect.lpstrDeviceType = TEXT("mpegvideo");
    openEffect.lpstrElementName = soundname;
    mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openEffect);
    SFXid = openEffect.wDeviceID;


    mciSendCommand(SFXid, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&playEffect);
    while (mciSendCommand(SFXid, MCI_STATUS, MCI_STATUS_MODE, (DWORD_PTR)&playEffect))
    {
        break;
    }
}
