//
//  CGImage.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <optional>
#include <memory>
#include <Data/Data.hpp>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class CGImage {
public:
    GPU_Image* pointee;

    CGImage(ptr<Data> sourceData);
    ~CGImage();

    Size size();
private:
    CGImage(GPU_Image* image, ptr<Data> sourceData);
    
    ptr<Data> sourceData;
};

}
