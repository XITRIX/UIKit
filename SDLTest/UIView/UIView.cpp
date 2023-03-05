//
//  UIView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#include <UIView/UIView.hpp>

namespace UIKit {

CALayer* UIView::initLayer() {
    return new CALayer();
}

UIView::UIView(Rect frame) {
    _layer = std::shared_ptr<CALayer>(initLayer());
//    layer->contentsScale = UIScreen.main.scale
    _layer->contentsScale = 1; // TODO: Fix value

    _layer->delegate = this;
    setFrame(frame);
}

void UIView::setFrame(Rect frame) {
    if (this->frame().size != frame.size) {
        setNeedsLayout();
    }
    _layer->setFrame(frame);
}

void UIView::setBounds(Rect bounds) {
    if (this->bounds().size != bounds.size) {
        setNeedsLayout();
    }
    _layer->setBounds(bounds);
}

void UIView::setCenter(Point position) {
    auto frame = this->frame();
    frame.setMidX(position.x);
    frame.setMidY(position.y);
    setFrame(frame);
}

Point UIView::center() const {
    auto frame = this->frame();
    return Point(frame.midX(), frame.midY());
}

void UIView::addSubview(std::shared_ptr<UIView> view) {
    setNeedsLayout();
    _layer->addSublayer(view->_layer);

    view->removeFromSuperview();
    subviews.push_back(view);
    view->superview = this->shared_from_this();
}

void UIView::insertSubviewAt(std::shared_ptr<UIView> view, int index) {
 // TODO: Need to implement
}

void UIView::removeFromSuperview() {
    auto superview = this->superview.lock();
    if (!superview) return;

    _layer->removeFromSuperlayer();

    superview->subviews.erase(std::remove(superview->subviews.begin(), superview->subviews.end(), shared_from_this()), superview->subviews.end());
    this->superview.reset();
    superview->setNeedsLayout();
}

// Animations
std::set<std::shared_ptr<CALayer>> UIView::layersWithAnimations;
std::shared_ptr<CABasicAnimationPrototype> UIView::currentAnimationPrototype;

void UIView::animateIfNeeded(Timer currentTime) {
    auto layersWithAnimationsCopy = layersWithAnimations;
    for (auto& layer: layersWithAnimationsCopy) {
        layer->animateAt(currentTime);
    }
}

void UIView::completePendingAnimations() {
    for (auto& layer: layersWithAnimations) {
        timeval now;
        gettimeofday(&now, nullptr);
        // FIXME: incorrect logic
        layer->animateAt(Timer(timevalInMilliseconds(now) + 1000000000));
//        $0.animate(at: Timer(startingAt: NSDate.distantFuture.timeIntervalSinceNow));
    }
}

std::shared_ptr<CABasicAnimation> UIView::actionForKey(std::string event) {
    auto prototype = UIView::currentAnimationPrototype;
    if (!prototype) { return nullptr; }

    auto keyPath = event;
    auto beginFromCurrentState = (prototype->animationGroup->options & UIViewAnimationOptions::beginFromCurrentState) == UIViewAnimationOptions::beginFromCurrentState;

    auto state = beginFromCurrentState ? (_layer->presentationOrSelf()) : _layer;

    auto fromValue = state->value(keyPath);
    
    if (fromValue.has_value()) {
        return prototype->createAnimation(keyPath, fromValue.value());
    }

    return nullptr;
}

void UIView::display(CALayer* layer) { }

}
