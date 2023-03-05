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

class UIView: public CALayerDelegate {
public:
    bool isUserInteractionEnabled = true;

    UIView(Rect frame);
    UIView(): UIView(Rect()) {}

    virtual CALayer* initLayer();

    void setFrame(Rect frame);
    Rect frame() const { return layer->getFrame(); }

    void setBounds(Rect bounds);
    Rect bounds() const { return layer->bounds(); }

    void setCenter(Point position);
    Point center() const;

    void setAlpha(float alpha) { layer->setOpacity(alpha); }
    float alpha() const { return layer->opacity(); }

    void setHidden(bool hidden) { layer->isHidden = hidden; }
    bool isHidden() const { return layer->isHidden; }

    void setBackbroundColor(std::optional<UIColor> backbroundColor) { layer->setBackgroundColor(backbroundColor); }
    std::optional<UIColor> backbroundColor() const { return layer->backgroundColor(); }

    void setNeedsLayout() { _needsLayout = true; }

    // Animations
    static std::set<std::shared_ptr<CALayer>> layersWithAnimations;
    static std::shared_ptr<CABasicAnimationPrototype> currentAnimationPrototype;

    static void animateIfNeeded(Timer currentTime);
    static void completePendingAnimations();

    std::shared_ptr<CABasicAnimation> actionForKey(std::string event) override;
    void display(CALayer* layer) override;

private:
    std::vector<std::shared_ptr<UIView>> subviews;
    std::shared_ptr<CALayer> layer;

    bool _needsLayout = true;
};

}
