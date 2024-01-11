//
//  main.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#ifndef __SWITCH__
#include <SDL_main.h>
#else
#include <switch.h>
#endif
#include "Runner/Runner.hpp"

int main(int argc, char* argv[]) {
#ifdef __SWITCH__
    socketInitializeDefault();              // Initialize sockets
    nxlinkStdio();
#endif

    int result = run();

#ifdef __SWITCH__
    socketExit();                           // Cleanup
#endif

    return result;
}
