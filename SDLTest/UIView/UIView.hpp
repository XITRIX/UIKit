//
//  UIView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <set>

namespace UIKit {

class UIView: public CALayerDelegate, public std::enable_shared_from_this<UIView> {
public:
    bool isUserInteractionEnabled = true;

    UIView(Rect frame);
    UIView(): UIView(Rect()) {}

    virtual CALayer* initLayer();
    std::shared_ptr<CALayer> layer() { return _layer; }

    void setFrame(Rect frame);
    Rect frame() const { return _layer->getFrame(); }

    void setBounds(Rect bounds);
    Rect bounds() const { return _layer->bounds(); }

    void setCenter(Point position);
    Point center() const;

    void setAlpha(float alpha) { _layer->setOpacity(alpha); }
    float alpha() const { return _layer->opacity(); }

    void setHidden(bool hidden) { _layer->_isHidden = hidden; }
    bool isHidden() const { return _layer->_isHidden; }

    void setTransform(NXAffineTransform transform);
    NXAffineTransform transform() const { return _layer->affineTransform(); }

    void setBackgroundColor(std::optional<UIColor> backbroundColor) { _layer->setBackgroundColor(backbroundColor); }
    std::optional<UIColor> backgroundColor() const { return _layer->backgroundColor(); }

    void setMask(std::shared_ptr<UIView> mask);
    std::shared_ptr<UIView> mask() { return _mask; }

    void addSubview(std::shared_ptr<UIView> view);
    void insertSubviewAt(std::shared_ptr<UIView> view, int index);
    void removeFromSuperview();

    // Layout
    void setNeedsDisplay() { _needsDisplay = true; }

    void setNeedsLayout() { _needsLayout = true; }

    void layoutIfNeeded();
    virtual void layoutSubviews();

    // Animations
    static std::set<std::shared_ptr<CALayer>> layersWithAnimations;
    static std::shared_ptr<CABasicAnimationPrototype> currentAnimationPrototype;

    static void animate(double duration,
                        double delay = 0.0,
                        UIViewAnimationOptions options = UIViewAnimationOptions::none,
                        std::function<void()> animations = [](){},
                        std::function<void(bool)> completion = [](bool res){});

    static void animate(double duration,
                        std::function<void()> animations = [](){});

    static void animate(double duration,
                        double delay,
                        double usingSpringWithDamping,
                        double initialSpringVelocity,
                        UIViewAnimationOptions options = UIViewAnimationOptions::none,
                        std::function<void()> animations = [](){},
                        std::function<void(bool)> completion = [](bool res){});

    static void animateIfNeeded(Timer currentTime);
    static void completePendingAnimations();

    std::shared_ptr<CABasicAnimation> actionForKey(std::string event) override;
    virtual void draw() {}
    void display(std::shared_ptr<CALayer> layer) override;

    // SDL
    void sdlDrawAndLayoutTreeIfNeeded(float parentAlpha = 1);

private:
    std::vector<std::shared_ptr<UIView>> subviews;
    std::weak_ptr<UIView> superview;
    std::shared_ptr<CALayer> _layer;
    std::shared_ptr<UIView> _mask;

    bool _needsLayout = true;
    bool _needsDisplay = true;
};

}
