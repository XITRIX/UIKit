//
//  CGImage.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <CGImage/CGImage.hpp>
#include <UIApplication/UIApplication.hpp>

namespace UIKit {

CGImage::CGImage(Size size) {
    pointee = GPU_CreateImage(size.width, size.height, GPU_FORMAT_RGBA);
    GPU_SetAnchor(pointee, 0, 0);
    GPU_SetBlendMode(pointee, GPU_BLEND_NORMAL_FACTOR_ALPHA);
}

CGImage::CGImage(GPU_Image* image, std::optional<Data> sourceData) {
    this->sourceData = sourceData;
    pointee = image;

    GPU_SetSnapMode(pointee, GPU_SNAP_POSITION_AND_DIMENSIONS);
    GPU_SetBlendMode(pointee, GPU_BLEND_NORMAL_FACTOR_ALPHA);
    GPU_SetImageFilter(pointee, GPU_FILTER_LINEAR);
}

CGImage::CGImage(Data sourceData) {
    auto data = sourceData;
    auto dataCount = data.count();

    auto rw = SDL_RWFromMem(data.data(), dataCount);
    auto gpuImagePtr = GPU_LoadImage_RW(rw, true);

    new (this) CGImage(gpuImagePtr, data);
}

CGImage::~CGImage() {
    GPU_FreeTarget(pointee->target);
    GPU_FreeImage(pointee);
}

Size CGImage::size() {
    return Size(pointee->w, pointee->h);
}

}
