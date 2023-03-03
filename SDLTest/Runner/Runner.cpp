//
//  Runner.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#include <Runner/Runner.hpp>

#include <algorithm>
#include <CALayer/CALayer.hpp>
#include <Utils/Utils.hpp>
#include <UIApplication/UIApplication.hpp>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

using namespace UIKit;

void Runner::refreshScreenResolution(Uint16 width, Uint16 height) {

    GPU_SetWindowResolution(width, height);
    GPU_SetVirtualResolution(renderer, width, height);
    if (rootLayer) {
        rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
    }
}

int Runner::startApp() {
#ifdef __SWITCH__
    Utils::resourcePath = "romfs:/";
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    Utils::resourcePath = std::string(SDL_GetBasePath()) + "/assets/";
#endif
#endif

    renderer = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    refreshScreenResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    UIApplication::currentRenderer = renderer;

    SDL_GameController *joystick;
    joystick = SDL_GameControllerOpen(0);

    // Event loop exit flag
    bool quit = false;

    rootLayer = std::make_shared<UIKit::CALayer>();
    rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
    rootLayer->backgroundColor = UIColor::systemBackground;
//    layer->setOpacity(1);

    auto layer1 = std::make_shared<UIKit::CALayer>();
//    layer1->anchorPoint = Point(0, 0);
    layer1->setFrame(Rect(120, 120, 280, 280));
    layer1->backgroundColor = UIColor::blue;
    layer1->cornerRadius = 16;
//    layer1->setOpacity(0.5f);
//    layer1->transform = NXTransform3D::translationBy(180, 180, 0);
//    layer1->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(180, 180, 0);


    auto layer2 = std::make_shared<UIKit::CALayer>();
//    layer2->anchorPoint = Point(0.5f, 0.5f);
    layer2->setFrame(Rect(-40, 40, 80, 80));
    layer2->backgroundColor = UIColor::red;
//    layer2->setOpacity(0.5f);
//    layer2->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::scaleBy(2.f, 1, 0); //* NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::translationBy(20, 0, 0);//.concat(NXTransform3D::scaleBy(1.5f, 1, 0));


    auto layer3 = std::make_shared<UIKit::CALayer>();
    layer3->setFrame(Rect(0, 0, 80, 80));
    layer3->backgroundColor = UIColor::black.withAlphaComponent(0.5f);
    layer3->transform = NXTransform3D::rotationBy(45, 0, 0, 1) * NXTransform3D::scale(0.5f);

    auto imageData = Data::fromPath("test3.png");
    auto image = std::make_shared<CGImage>(imageData);

//    layer3->contents = image;

    rootLayer->addSublayer(layer1);
//    layer1->setMask(layer2);
    layer1->addSublayer(layer2);
//    layer2->setMask(layer3);
    layer2->addSublayer(layer3);

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

//        auto ssfbo = std::make_shared<CGImage>(Size(renderer->base_w, renderer->base_h));
//        GPU_GetTarget(ssfbo->pointee);
//        GPU_SetActiveTarget(ssfbo->pointee->target);
//        GPU_Clear(ssfbo->pointee->target);

        rootLayer->render(renderer);

//        GPU_SetActiveTarget(renderer);
//
//        auto rect = GPU_MakeRect(0, 0, renderer->w, renderer->h);
//        GPU_BlitRect(ssfbo->pointee, NULL, renderer, &rect);

        // Update screen
        GPU_Flip(renderer);
    }

    SDL_GameControllerClose(joystick);

    GPU_Quit();

    return 0;
}
