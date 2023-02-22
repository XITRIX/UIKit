//
//  CALayer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <Utils/Utils.hpp>
#include <UIColor/UIColor.hpp>
#include <CGImage/CGImage.hpp>
#include <Geometry/Geometry.hpp>
#include <NXTransform3D/NXTransform3D.hpp>

namespace UIKit {

class CALayer: public std::enable_shared_from_this<CALayer> {
public:
    float zPosition = 0;
    Point position;
    Rect bounds;

    UIColor backgroundColor;
    ptr<UIKit::CGImage> contents;

    NXTransform3D transform = NXTransform3D::identity;

    CALayer();

    virtual void draw(GPU_Target* renderer);
    void render(GPU_Target* renderer);
//    void render(GPU_Target* renderer, Point globalOffset);

    Rect getFrame();
    void setFrame(Rect frame);

    void setOpacity(float opacity);
    float getOpacity() const;

    void addSublayer(ptr<CALayer> layer);
    void insertSublayerAt(ptr<CALayer> layer, int index);
    void insertSublayerAbove(ptr<CALayer> layer, ptr<CALayer> sibling);
    void insertSublayerBelow(ptr<CALayer> layer, ptr<CALayer> sibling);

    void removeFromSuperlayer();

private:
    float opacity = 1;
    wptr<CALayer> superlayer;
    std::vector<ptr<CALayer>> sublayers;
};

}

