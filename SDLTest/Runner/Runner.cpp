//
//  Runner.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#include <Runner/Runner.hpp>

#include <algorithm>
#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

using namespace UIKit;

void Runner::refreshScreenResolution(Uint16 width, Uint16 height) {
    GPU_SetWindowResolution(width, height);
    GPU_SetVirtualResolution(renderer, width, height);
}

int Runner::startApp() {
//    SDL_Init(SDL_INIT_EVERYTHING);
    renderer = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    refreshScreenResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    UIApplication::currentRenderer = renderer;

    SDL_GameController *joystick;
    joystick = SDL_GameControllerOpen(0);

    // Event loop exit flag
    bool quit = false;

    auto rootLayer = std::make_shared<UIKit::CALayer>();
    rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
    rootLayer->backgroundColor = UIColor::systemBackground;
//    layer->setOpacity(1);

    auto layer1 = std::make_shared<UIKit::CALayer>();
//    layer1->anchorPoint = Point(0, 0);
    layer1->setFrame(Rect(20, 20, 280, 280));
    layer1->backgroundColor = UIColor::orange;
//    layer1->setOpacity(0.1f);
    layer1->transform = NXTransform3D::translationBy(0, 0, 0);


    auto layer2 = std::make_shared<UIKit::CALayer>();
    layer2->anchorPoint = Point(0.5f, 0.5f);
    layer2->setFrame(Rect(0, 40, 80, 80));
    layer2->backgroundColor = UIColor::red;
//    layer2->setOpacity(0.5f);
    layer2->transform = NXTransform3D::scaleBy(2.f, 1, 0); //* NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::translationBy(0, 40, 0);//.concat(NXTransform3D::scaleBy(1.5f, 1, 0));

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
                    refreshScreenResolution(event.window.data1, event.window.data2);
                    rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        GPU_Clear(renderer);

        rootLayer->render(renderer);


        // Update screen
        GPU_Flip(renderer);
    }

    SDL_GameControllerClose(joystick);

    GPU_Quit();

    return 0;
}
