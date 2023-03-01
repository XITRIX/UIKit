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
#include <NXAffineTransform/NXAffineTransform.hpp>
#include <ContentsGravityTransformation/ContentsGravityTransformation.hpp>

namespace UIKit {

class CALayer: public std::enable_shared_from_this<CALayer> {
public:
    float zPosition = 0;
    Point anchorPoint = Point(0.5f, 0.5f);
    Point position;
    Rect bounds;
    float cornerRadius = 0;
    bool allowsGroupOpacity = true;

    /// Defaults to 1.0 but if the layer is associated with a view,
    /// the view sets this value to match the screen.
    float contentsScale = 1.0f;

    CALayerContentsGravity contentsGravity = CALayerContentsGravity::resize;

    UIColor backgroundColor;
    ptr<CGImage> contents;

    NXTransform3D transform = NXTransform3D::identity;

    CALayer();
    ~CALayer();

    virtual void draw(GPU_Target* renderer);
    void render(GPU_Target* renderer);
//    void render(GPU_Target* renderer, Point globalOffset);

    Rect getFrame();
    void setFrame(Rect frame);

    void setOpacity(float opacity);
    float getOpacity() const;

    void setMask(ptr<CALayer> mask);
    ptr<CALayer> getMask() const;

    void addSublayer(ptr<CALayer> layer);
    void insertSublayerAt(ptr<CALayer> layer, int index);
    void insertSublayerAbove(ptr<CALayer> layer, ptr<CALayer> sibling);
    void insertSublayerBelow(ptr<CALayer> layer, ptr<CALayer> sibling);

    void removeFromSuperlayer();

    NXAffineTransform affineTransform();
    void setAffineTransform(NXAffineTransform transform);

private:
    float opacity = 1;
    wptr<CALayer> superlayer;
    std::vector<ptr<CALayer>> sublayers;
    ptr<CALayer> mask;
    GPU_Image* groupingFBO = nullptr;
    ptr<CGImage> maskFBO;

    void refreshGroupingFBO();

    Rect getRenderedBoundsRelativeToAnchorPoint();
};

}

