//
//  UIRenderer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <Geometry/Geometry.hpp>
#include <Timer/Timer.hpp>
#include <memory>

#ifdef main
#undef main
#endif

namespace UIKit {

class UIWindow;
class UIApplicationDelegate;
class UIRenderer {
public:

    UIRenderer();

    static std::shared_ptr<UIRenderer> main() { return _main; }
    Rect bounds() { return _bounds; }
    float scale()  { return _scale; }

    void render(std::shared_ptr<UIWindow> window, Timer frameTimer);

    std::optional<Rect> clippingRect() { return _clippingRect; }
    void setClippingRect(std::optional<Rect> newValue);

private:
    GPU_Target* rawPointer;

    Rect _bounds;
    float _scale;
    std::optional<Rect> _clippingRect;

    static std::shared_ptr<UIRenderer> _main;

    void refreshScreenResolution(Uint16 width, Uint16 height);
    int getFps();

    friend class UIApplication;
    friend class Renderer;
    friend int UIApplicationMain(std::shared_ptr<UIApplicationDelegate> appDelegate);
};

}
