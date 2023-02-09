//
//  main.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <Global.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <CALayer/CALayer.hpp>

int main(int argc, const char * argv[]) {

    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 600;

    SDL_Init(SDL_INIT_EVERYTHING);

    // Create window
    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SCREEN_WIDTH, SCREEN_HEIGHT,
                                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    globalRenderer = renderer;

    UIKit::CALayer* layer = new UIKit::CALayer();
    layer->backgroundColor = UIKit::UIColor(255, 0, 0, 255);
    layer->setFrame(UIKit::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    // Event loop exit flag
    bool quit = false;
    bool drawn = false;

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
        auto Width = DM.w;
        auto Height = DM.h;

        auto frame = layer->getFrame();
        SDL_Rect rect;
        rect.x = frame.origin.x;
        rect.y = frame.origin.y;
        rect.w = frame.size.width;
        rect.h = frame.size.height;

        /// - Drawing texture
//        if (!drawn) {
//            drawn = true;
            layer->draw(renderer);
//        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopyEx(renderer, layer->contents->pointee, NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);

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
