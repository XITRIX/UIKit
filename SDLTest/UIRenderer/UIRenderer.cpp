//
//  UIRenderer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIRenderer/UIRenderer.hpp>
#include <UIApplication/UIApplication.hpp>
#include <Renderer/Renderer.hpp>
#include <Utils/Utils.hpp>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

namespace UIKit {

std::shared_ptr<UIRenderer> UIRenderer::_main;

void UIRenderer::refreshScreenResolution(Uint16 width, Uint16 height) {

    GPU_SetWindowResolution(width, height);
    GPU_SetVirtualResolution(rawPointer, width, height);

    _bounds.size = Size(rawPointer->w, rawPointer->h);
    if (UIApplication::shared && !UIApplication::shared->keyWindow.expired()) {
        UIApplication::shared->keyWindow.lock()->setFrame(Rect(0, 0, rawPointer->w, rawPointer->h));
    }
}


UIRenderer::UIRenderer() {
    if (_main) { exit(1); }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    rawPointer = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    refreshScreenResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
    _scale = float(rawPointer->base_h) / float(rawPointer->h);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

}

void UIRenderer::render(std::shared_ptr<UIWindow> window, Timer frameTimer) {
    UIView::animateIfNeeded(frameTimer);

    // TODO: Remove refreshing every frame
    refreshScreenResolution(window->bounds().size.width, window->bounds().size.height);

    window->sdlDrawAndLayoutTreeIfNeeded();

    if (!CALayer::layerTreeIsDirty) {
        SDL_Delay(1);
        return;
    }

    CALayer::layerTreeIsDirty = false;

    GPU_Clear(rawPointer);

    window->layer()->render(rawPointer);

    Renderer::shared()->draw([this](auto vg) {
        nvgFontSize(vg, 21);
        nvgFillColor(vg, UIColor::black.nvgColor());
        nvgText(vg, 20, 20, ("FPS: " + std::to_string(getFps())).c_str(), nullptr);
    });

    // Update screen
    GPU_Flip(rawPointer);
}

int UIRenderer::getFps() {
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

}

