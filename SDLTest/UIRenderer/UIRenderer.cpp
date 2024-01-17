//
//  UIRenderer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIRenderer/UIRenderer.hpp>
#include <UIApplication/UIApplication.hpp>
#include <Renderer/Renderer.hpp>
#include <Tools/Tools.hpp>

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
    GPU_SetActiveTarget(rawPointer);
//    if (SDL_GL_SetSwapInterval(-1) == -1) {
//        if (SDL_GL_SetSwapInterval(1) == -1) {
//            printf("FAILED\n");
//        }
//    }

    if (!rawPointer) {
        printf("%s\n", GPU_PopErrorCode().details);
        printf("%s\n", SDL_GetError());
        exit(-1);
    }

    SDL_Rect gScreenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
#if !defined(RESIZABLE_SCREEN) && !defined(__SWITCH__)
    //Get device display mode
    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
    {
        gScreenRect.w = displayMode.w;
        gScreenRect.h = displayMode.h;
    }
#endif
    // TODO: Remove int rounding when initial screen resolution will be fixed
    _scale = float(rawPointer->base_h) / float(rawPointer->h);
    refreshScreenResolution(gScreenRect.w, gScreenRect.h);

    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    SDL_GameControllerOpen(0); // TODO: Move to another place

#if defined(RESIZABLE_SCREEN)
    SDL_AddEventWatch(resizingEventWatcher, nullptr);
#endif
}

int UIRenderer::resizingEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        auto currentTime = Timer();
        UIRenderer::main()->refreshScreenResolution(event->window.data1, event->window.data2);
        UIRenderer::main()->render(UIApplication::shared->keyWindow.lock(), currentTime);
    }
    return 0;
}

void UIRenderer::render(const std::shared_ptr<UIWindow>& window, Timer frameTimer) {
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

void UIRenderer::setClippingRect(std::optional<Rect> newValue) {
    _clippingRect = newValue;

    if (_clippingRect.has_value()) {
        GPU_SetClipRect(rawPointer, _clippingRect.value().gpuRect());
    } else {
        GPU_UnsetClip(rawPointer);
    }
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

