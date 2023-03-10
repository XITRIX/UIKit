//
//  UIView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>
#include <UIEvent/UIEvent.hpp>
#include <UIViewContentMode/UIViewContentMode.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIResponder/UIResponder.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <set>

namespace UIKit {

class UIViewController;
class UIView: public UIResponder, public CALayerDelegate, public std::enable_shared_from_this<UIView> {
public:
    std::string tag;
    bool isUserInteractionEnabled = true;

    UIView(Rect frame);
    UIView(): UIView(Rect()) {}

    virtual CALayer* initLayer();
    std::shared_ptr<CALayer> layer() { return _layer; }

    std::shared_ptr<UIResponder> next() override;

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

    void setClipsToBounds(bool clipsToBounds) { _layer->_masksToBounds = clipsToBounds; }
    bool clipsToBounds() const { return _layer->_masksToBounds; }

    void setTransform(NXAffineTransform transform);
    NXAffineTransform transform() const { return _layer->affineTransform(); }

    void setBackgroundColor(std::optional<UIColor> backbroundColor) { _layer->setBackgroundColor(backbroundColor); }
    std::optional<UIColor> backgroundColor() const { return _layer->backgroundColor(); }

    void setMask(std::shared_ptr<UIView> mask);
    std::shared_ptr<UIView> mask() const { return _mask; }

    void setContentMode(UIViewContentMode mode);
    UIViewContentMode contentMode() const { return _contentMode; }

    std::vector<std::shared_ptr<UIGestureRecognizer>>* gestureRecognizers() { return &_gestureRecognizers; }
    void addGestureRecognizer(std::shared_ptr<UIGestureRecognizer> gestureRecognizer);

    void addSubview(std::shared_ptr<UIView> view);
    void insertSubviewAt(std::shared_ptr<UIView> view, int index);
    void removeFromSuperview();

    std::vector<std::shared_ptr<UIView>> subviews() const { return _subviews; }
    std::weak_ptr<UIView> superview() const { return _superview; }

    // Layout
    void setNeedsDisplay() { _needsDisplay = true; }
    void setNeedsLayout() { _needsLayout = true; }

    void layoutIfNeeded();
    virtual void layoutSubviews();

    virtual Size sizeThatFits(Size size);
    virtual void sizeToFit();

    // Touch
    Point convertToView(Point point, std::shared_ptr<UIView> toView);
    Point convertFromView(Point point, std::shared_ptr<UIView> fromView);
    virtual std::shared_ptr<UIView> hitTest(Point point, UIEvent* withEvent);
    virtual bool point(Point insidePoint, UIEvent* withEvent);

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
    virtual void display(std::shared_ptr<CALayer> layer) override;

    // SDL
    void sdlDrawAndLayoutTreeIfNeeded(float parentAlpha = 1);

private:
    std::vector<std::shared_ptr<UIGestureRecognizer>> _gestureRecognizers;
    std::vector<std::shared_ptr<UIView>> _subviews;
    std::weak_ptr<UIView> _superview;
    std::shared_ptr<CALayer> _layer;
    std::shared_ptr<UIView> _mask;
    UIViewContentMode _contentMode;
    std::weak_ptr<UIViewController> _parentController;

    bool _needsLayout = true;
    bool _needsDisplay = true;

    void setSuperview(std::shared_ptr<UIView> superview);
    bool anyCurrentlyRunningAnimationsAllowUserInteraction();

    friend class UIViewController;
};

}
