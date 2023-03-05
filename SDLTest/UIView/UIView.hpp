//
//  UIView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
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

    void setHidden(bool hidden) { _layer->isHidden = hidden; }
    bool isHidden() const { return _layer->isHidden; }

    void setBackgroundColor(std::optional<UIColor> backbroundColor) { _layer->setBackgroundColor(backbroundColor); }
    std::optional<UIColor> backgroundColor() const { return _layer->backgroundColor(); }

    void setNeedsLayout() { _needsLayout = true; }

    void addSubview(std::shared_ptr<UIView> view);
    void insertSubviewAt(std::shared_ptr<UIView> view, int index);
    void removeFromSuperview();

    // Animations
    static std::set<std::shared_ptr<CALayer>> layersWithAnimations;
    static std::shared_ptr<CABasicAnimationPrototype> currentAnimationPrototype;

    static void animateIfNeeded(Timer currentTime);
    static void completePendingAnimations();

    std::shared_ptr<CABasicAnimation> actionForKey(std::string event) override;
    void display(CALayer* layer) override;

private:
    std::vector<std::shared_ptr<UIView>> subviews;
    std::weak_ptr<UIView> superview;
    std::shared_ptr<CALayer> _layer;

    bool _needsLayout = true;
};

}
