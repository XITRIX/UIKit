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
    virtual void display(std::shared_ptr<CALayer> layer) = 0;
};

class CALayer: public enable_shared_from_this<CALayer> {
public:
    float zPosition = 0;

    bool allowsGroupOpacity = true;
    CALayerDelegate* delegate;

    /// Defaults to 1.0 but if the layer is associated with a view,
    /// the view sets this value to match the screen.
    float contentsScale = 1.0f;

    CALayerContentsGravity contentsGravity = CALayerContentsGravity::resize;

    CALayer();
    CALayer(CALayer* layer);
    ~CALayer();

    virtual void draw(NVGcontext* context);
    void render(GPU_Target* renderer);
//    void render(GPU_Target* renderer, Point globalOffset);

    Rect getFrame();
    void setFrame(Rect frame);

    void setBounds(Rect bounds);
    Rect bounds() const { return _bounds; }

    void setPosition(Point position);
    Point position() const { return _position; }

    void setOpacity(float opacity);
    float opacity() const { return _opacity; }

    void setAnchorPoint(Point anchorPoint);
    Point anchorPoint() const { return _anchorPoint; }

    void setTransform(NXTransform3D transform);
    NXTransform3D transform() const { return _transform; }

    void setCornerRadius(float cornerRadius);
    float cornerRadius() const { return _cornerRadius; }

    void setBackgroundColor(std::optional<UIColor> backgroundColor);
    std::optional<UIColor> backgroundColor() const { return _backgroundColor; }

    void setMask(std::shared_ptr<CALayer> mask);
    std::shared_ptr<CALayer> getMask() const;

    void setHidden(bool hidden);
    bool isHidden() const { return _isHidden; }

    void setContents(std::shared_ptr<CGImage> contents);
    std::shared_ptr<CGImage> contents() { return _contents; }

    bool masksToBounds() { return _masksToBounds; }
    void setMasksToBounds(bool newValue) { _masksToBounds = newValue; }

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

    bool needsDisplay() { return _needsDisplay; }
    void setNeedsDisplay() { _needsDisplay = true; }
    void display();

    // Animations
    void add(std::shared_ptr<CABasicAnimation> animation, std::string keyPath);
    void removeAnimation(std::string forKey);
    void removeAllAnimations();
    void onWillSet(std::string keyPath);
    void onDidSetAnimations(bool wasEmpty);
    std::optional<AnimatableProperty> value(std::string forKeyPath);

    void animateAt(Timer currentTime);

    /**
     Indicates whether a layer somewhere has changed since the last render pass.

     The current implementation of this is quite simple and doesn't check whether the layer is actually in
     the layer hierarchy or not. In theory this means that we're wasting render passes if users frequently
     update layers that aren't in the tree. In practice it's not expected that UIKit users would do that
     often enough for us to care about it.
    **/
    static bool layerTreeIsDirty;

private:
    // Animatable
    Point _anchorPoint = Point(0.5f, 0.5f);
    Point _position;
    Rect _bounds;
    float _opacity = 1;
    NXTransform3D _transform = NXTransform3D::identity;
    std::optional<UIColor> _backgroundColor;
    float _cornerRadius = 0;

    bool _isHidden = false;
    bool _needsDisplay = true;

    bool _masksToBounds = false;
    std::shared_ptr<CGImage> _contents;

    std::weak_ptr<CALayer> superlayer;
    std::vector<std::shared_ptr<CALayer>> sublayers;
    std::shared_ptr<CALayer> mask;
    GPU_Image* groupingFBO = nullptr;
    std::shared_ptr<CGImage> maskFBO;

    std::shared_ptr<CALayer> _presentation;
    std::map<std::string, std::shared_ptr<CABasicAnimation>> animations;

    std::shared_ptr<CALayer> presentationOrSelf();

    bool isPresentationForAnotherLayer = false;

    /// We disable animation on parameters of views / layers that haven't been rendered yet.
    /// This is both a performance optimization (avoids lots of animations at the start)
    /// as well as a correctness fix (matches iOS behaviour). Maybe there's a better way though?
    bool hasBeenRenderedInThisPartOfOverallLayerHierarchy = false;
    void update(std::shared_ptr<CALayer> presentation, std::shared_ptr<CABasicAnimation> animation, float progress);

    void refreshGroupingFBO();

    Rect getRenderedBoundsRelativeToAnchorPoint();

    void drawNVG(std::function<void(NVGcontext*)> draw);

    friend class UIView;
};

}

