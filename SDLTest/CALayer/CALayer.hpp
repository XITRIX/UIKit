//
//  CALayer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <any>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <nanovg.h>
#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include <UIColor/UIColor.hpp>
#include <CGImage/CGImage.hpp>
#include <Geometry/Geometry.hpp>
#include <CABasicAnimation/CABasicAnimation.hpp>
#include <NXTransform3D/NXTransform3D.hpp>
#include <NXAffineTransform/NXAffineTransform.hpp>
#include <ContentsGravityTransformation/ContentsGravityTransformation.hpp>

namespace UIKit {

class CALayerDelegate {
public:
    virtual std::shared_ptr<CABasicAnimation> actionForKey(std::string event) = 0;
    virtual void display(CALayer* layer) = 0;
};

class CALayer: public std::enable_shared_from_this<CALayer> {
public:
    float zPosition = 0;
    Point anchorPoint = Point(0.5f, 0.5f);
    Point position;
    Rect bounds;
    float cornerRadius = 0;
    bool allowsGroupOpacity = true;
    bool isHidden = false;
    std::shared_ptr<CALayerDelegate> delegate;

    /// Defaults to 1.0 but if the layer is associated with a view,
    /// the view sets this value to match the screen.
    float contentsScale = 1.0f;

    CALayerContentsGravity contentsGravity = CALayerContentsGravity::resize;

    std::optional<UIColor> backgroundColor;
    std::shared_ptr<CGImage> contents;

    NXTransform3D transform = NXTransform3D::identity;

    CALayer();
    CALayer(CALayer* layer);
    ~CALayer();

    virtual void draw(NVGcontext* context);
    void render(GPU_Target* renderer);
//    void render(GPU_Target* renderer, Point globalOffset);

    Rect getFrame();
    void setFrame(Rect frame);

    void setOpacity(float opacity);
    float getOpacity() const;

    void setMask(std::shared_ptr<CALayer> mask);
    std::shared_ptr<CALayer> getMask() const;

    void addSublayer(std::shared_ptr<CALayer> layer);
    void insertSublayerAt(std::shared_ptr<CALayer> layer, int index);
    void insertSublayerAbove(std::shared_ptr<CALayer> layer, std::shared_ptr<CALayer> sibling);
    void insertSublayerBelow(std::shared_ptr<CALayer> layer, std::shared_ptr<CALayer> sibling);

    void removeFromSuperlayer();

    NXAffineTransform affineTransform();
    void setAffineTransform(NXAffineTransform transform);

    CALayer* copy();

    std::shared_ptr<CAAction> actionForKey(std::string event);

    std::shared_ptr<CALayer> createPresentation();

    static std::shared_ptr<CABasicAnimation> defaultActionForKey(std::string event);
    static float defaultAnimationDuration;

    std::shared_ptr<CALayer> presentation() { return _presentation; }

    // Animations
    void add(std::shared_ptr<CABasicAnimation> animation, std::string keyPath);
    void removeAnimation(std::string forKey);
    void removeAllAnimations();
    void onWillSet(std::string keyPath);
    void onDidSetAnimations(bool wasEmpty);
    std::optional<AnimatableProperty> value(std::string forKeyPath);

    void animateAt(Timer currentTime);

private:
    float opacity = 1;
    std::weak_ptr<CALayer> superlayer;
    std::vector<std::shared_ptr<CALayer>> sublayers;
    std::shared_ptr<CALayer> mask;
    GPU_Image* groupingFBO = nullptr;
    std::shared_ptr<CGImage> maskFBO;

    std::shared_ptr<CALayer> _presentation;
    std::map<std::string, std::shared_ptr<CABasicAnimation>> animations;

    bool isPresentationForAnotherLayer = false;

    /// We disable animation on parameters of views / layers that haven't been rendered yet.
    /// This is both a performance optimization (avoids lots of animations at the start)
    /// as well as a correctness fix (matches iOS behaviour). Maybe there's a better way though?
    bool hasBeenRenderedInThisPartOfOverallLayerHierarchy = false;
    void update(std::shared_ptr<CALayer> presentation, std::shared_ptr<CABasicAnimation> animation, float progress);

    void refreshGroupingFBO();

    Rect getRenderedBoundsRelativeToAnchorPoint();

    void drawNVG(std::function<void(NVGcontext*)> draw);
};

}

