//
//  Runner.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#include <Runner/Runner.hpp>

#include <algorithm>
#include <UIView/UIView.hpp>
#include <CALayer/CALayer.hpp>
#include <Renderer/Renderer.hpp>
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

    rootLayer = std::make_shared<UIView>();
    rootLayer->setFrame(Rect(0, 0, renderer->w, renderer->h));
    rootLayer->setBackgroundColor(UIColor::systemBackground);
//    layer->setOpacity(1);

    auto layer1 = std::make_shared<UIKit::UIView>();
//    layer1->anchorPoint = Point(0, 0);
    layer1->setFrame(Rect(44, 44, 280, 280));
    layer1->setBackgroundColor(UIColor::blue);
    layer1->layer()->cornerRadius = 16;
    layer1->setAlpha(0.5f);
//    layer1->transform = NXTransform3D::translationBy(180, 180, 0);
//    layer1->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(180, 180, 0);

    auto group = std::make_shared<UIViewAnimationGroup>(UIViewAnimationOptions::curveLinear, [](auto i){});
    UIView::currentAnimationPrototype = std::make_shared<CABasicAnimationPrototype>(4, 4, group);
//    layer1->setPosition(Point(644, 480));
    auto frame = layer1->frame();
    frame.origin = Point(644, 480);
    layer1->setFrame(frame);
    UIView::currentAnimationPrototype = nullptr;


    auto layer2 = std::make_shared<UIKit::UIView>();
//    layer2->anchorPoint = Point(0.5f, 0.5f);
    layer2->setFrame(Rect(40, 40, 80, 80));
    layer2->setBackgroundColor(UIColor::red);//.withAlphaComponent(0.3f);
//    layer2->setOpacity(0.5f);
//    layer2->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::scaleBy(2.f, 1, 0); //* NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::translationBy(20, 0, 0);//.concat(NXTransform3D::scaleBy(1.5f, 1, 0));


    auto layer3 = std::make_shared<UIKit::UIView>();
    layer3->setFrame(Rect(0, 0, 80, 80));
    layer3->setBackgroundColor(UIColor::black);//.withAlphaComponent(0.5f);
    layer3->layer()->cornerRadius = 16;
//    layer3->setTransform(NXTransform3D::rotationBy(45, 0, 0, 1) * NXTransform3D::scale(0.5f));

    auto imageData = Data::fromPath("test3.png");
    auto image = std::make_shared<CGImage>(imageData);

    layer3->layer()->contents = image;

    rootLayer->addSubview(layer1);
//    layer1->setMask(layer2);
    layer1->addSubview(layer2);
//    layer2->setMask(layer3);
    layer2->addSubview(layer3);

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

        auto frameTimer = Timer();
        UIView::animateIfNeeded(frameTimer);

        GPU_Clear(renderer);

//        auto ssfbo = std::make_shared<CGImage>(Size(renderer->base_w, renderer->base_h));
//        GPU_GetTarget(ssfbo->pointee);
//        GPU_SetActiveTarget(ssfbo->pointee->target);
//        GPU_Clear(ssfbo->pointee->target);

        rootLayer->layer()->render(renderer);

        Renderer::shared()->draw([this](auto vg) {
//            nvgFontSize(vg, 22);
//            nvgFillColor(vg, UIColor::black.nvgColor());
//            nvgText(vg, 20, 20, "Test text", nullptr);

            nvgFontSize(vg, 21);
            nvgFillColor(vg, UIColor::black.nvgColor());
            nvgText(vg, 20, 20, ("FPS: " + std::to_string(getFps())).c_str(), nullptr);
        });

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

int Runner::getFps() {
    static double counter = 0;
    static double res = 0;
    static double prevres = 0;
    static double prevt = getCPUTimeUsec();
    double t = getCPUTimeUsec();
    double dt = t - prevt;
    prevt = t;

    res += dt;
    counter++;

    if (counter >= 60) {
        prevres = res;
        res = 0;
        counter = 0;
    }

    int fps = 1 / (prevres / 1000000.0 / 60.0);
    return fps;
}
