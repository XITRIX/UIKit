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
#include <Utils/Utils.hpp>
#include <UIColor/UIColor.hpp>
#include <CGImage/CGImage.hpp>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class CALayer: public std::enable_shared_from_this<CALayer> {
public:
    Point position;
    Rect bounds;

    UIColor backgroundColor;
    ptr<UIKit::CGImage> contents;

    CALayer();

    virtual void draw(SDL_Renderer* renderer);
    void internalDraw(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, Point globalOffset);

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
    void updateContents();
    float opacity = 1;
    wptr<CALayer> superlayer;
    std::vector<ptr<CALayer>> sublayers;
};

}

