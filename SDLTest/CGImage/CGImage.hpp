//
//  CGImage.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <memory>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class CGImage {
public:
    GPU_Image* pointee;

    CGImage(Size size);
    ~CGImage();

    Size size();
};

}
