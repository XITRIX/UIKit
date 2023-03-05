//
//  Runner.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#pragma once

#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <UIView/UIView.hpp>

namespace UIKit {

class Runner {
public:
    int startApp();
private:
    GPU_Target* renderer;
    std::shared_ptr<UIView> rootLayer = nullptr;
    void refreshScreenResolution(Uint16 width, Uint16 height);
    int getFps();
};

}
