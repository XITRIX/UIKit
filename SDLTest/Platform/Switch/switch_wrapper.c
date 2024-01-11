//
// Created by Даниил Виноградов on 11.01.2024.
//

#include <stdio.h>
#include <switch.h>
#include <unistd.h>

static int nxlink_sock = -1;

void userAppInit()
{
    printf("userAppInit\n");
    appletLockExit();

    // Init network
    SocketInitConfig cfg = *(socketGetDefaultInitConfig());
    AppletType at        = appletGetAppletType();
    if (at == AppletType_Application || at == AppletType_SystemApplication)
    {
        cfg.num_bsd_sessions = 12; // default is 3
        cfg.sb_efficiency    = 8; // default is 4
        socketInitialize(&cfg);
    }
    else
    {
        cfg.num_bsd_sessions = 2;
        cfg.sb_efficiency    = 1;
        socketInitialize(&cfg);
    }

// #ifdef DEBUG
    nxlink_sock = nxlinkStdio();
// #endif

    romfsInit();
    plInitialize(PlServiceType_User);
    setsysInitialize();
    setInitialize();
    psmInitialize();
    nifmInitialize(NifmServiceType_User);
}

void userAppExit()
{
    printf("userAppExit\n");

    // 网络状态
    nifmExit();
    // 电源状态
    psmExit();
    // 设置（比如当前语言）
    setExit();
    // 系统设置（比如系统当前语言）
    setsysExit();
    // 系统字体
    plExit();

    romfsExit();

    if (nxlink_sock != -1)
        close(nxlink_sock);

    socketExit();

    appletUnlockExit();
}nk_sock = -1;