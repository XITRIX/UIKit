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
//    pointee = SDL_CreateTexture(UIApplication::currentRenderer, SDL_PIXELFORMAT_RGBA8888,
//                                SDL_TEXTUREACCESS_TARGET, (int)size.width, (int)size.height);
    pointee = GPU_CreateImage(size.width, size.width, GPU_FORMAT_RGBA);
//    SDL_SetTextureBlendMode(pointee, SDL_BLENDMODE_BLEND);
    GPU_SetBlendMode(pointee, GPU_BLEND_NORMAL);
}

CGImage::~CGImage() {
//    SDL_DestroyTexture(pointee);
    GPU_FreeImage(pointee);
}

Size CGImage::size() {
//    int width, height;
//    SDL_QueryTexture(pointee, NULL, NULL, &width, &height);

    return Size(pointee->w, pointee->h);
}

}
