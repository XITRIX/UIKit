//
//  CGImage.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL.h>
#include <SDL_gpu.h>
#include <optional>
#include <memory>
#include <Data/Data.hpp>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class CGImage {
public:
    GPU_Image* pointee;

    CGImage(Size size);
    CGImage(const Data& sourceData);
    CGImage(SDL_Surface* surface);
    CGImage(GPU_Image* image, std::optional<Data> sourceData);
    CGImage(GPU_Image* image): CGImage(image, std::nullopt) {}
    ~CGImage();

    [[nodiscard]] Size size() const;
private:
    std::optional<Data> sourceData;
};

}
