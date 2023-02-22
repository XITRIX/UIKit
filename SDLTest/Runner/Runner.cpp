//
//  Runner.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#include <Runner/Runner.hpp>

#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

using namespace UIKit;

int startApp() {
//    SDL_Init(SDL_INIT_EVERYTHING);
    GPU_Target* renderer = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    UIApplication::currentRenderer = renderer;

    SDL_GameController *joystick;
    joystick = SDL_GameControllerOpen(0);

    // Event loop exit flag
    bool quit = false;

    auto rootLayer = std::make_shared<UIKit::CALayer>();
    rootLayer->backgroundColor = UIColor::systemBackground;
//    layer->setOpacity(1);

    auto layer1 = std::make_shared<UIKit::CALayer>();
    layer1->setFrame(Rect(120, 100, 280, 280));
    layer1->backgroundColor = UIColor::orange;


    auto layer2 = std::make_shared<UIKit::CALayer>();
    layer2->setFrame(Rect(90, 90, 80, 80));
    layer2->backgroundColor = UIColor::red;
//    layer2->transform = NXTransform3D::translationBy(100, 150, 0).concat(NXTransform3D::scaleBy(1.5f, 1, 0));

    rootLayer->addSublayer(layer1);
    layer1->addSublayer(layer2);
//    layer2->render(renderer);

    // Event loop
    while(!quit)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // User requests quit
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q) {
                    quit = true;
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    quit = true;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    GPU_SetWindowResolution(event.window.data1, event.window.data2);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        GPU_SetActiveTarget(renderer);


        rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
        rootLayer->render(renderer);

        GPU_Clear(renderer);

        rootLayer->render(renderer);

        // Update screen
        GPU_Flip(renderer);
    }

    SDL_GameControllerClose(joystick);

    GPU_Quit();

    return 0;
}
