//
//  main2.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <stdio.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

using namespace UIKit;

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SCREEN_WIDTH, SCREEN_HEIGHT,
                                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    UIApplication::currentRenderer = renderer;

    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

    // Square position: In the middle of the screen
    squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
    squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;


    // Event loop exit flag
    bool quit = false;

    auto size = Size(SCREEN_WIDTH, SCREEN_HEIGHT);

    auto layer = std::make_shared<UIKit::CALayer>();
    layer->backgroundColor = UIColor::systemBackground;
//    layer->setOpacity(1);

    auto layer2 = std::make_shared<UIKit::CALayer>();
    layer2->setFrame(Rect(120, 100, 80, 80));
    layer2->backgroundColor = UIColor::orange;
//    layer2->setOpacity(1);

    layer->addSublayer(layer2);
    layer2->internalDraw(renderer);

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }

        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);

        layer->setFrame(Rect(0, 0, DM.w, DM.h));
        layer->internalDraw(renderer);

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        layer->render(renderer, Point());

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;
}
