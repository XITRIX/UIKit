//
//  UIView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#include <UIView/UIView.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIViewController/UIViewController.hpp>
#include <CASpringAnimationPrototype/CASpringAnimationPrototype.hpp>

namespace UIKit {

CALayer* UIView::initLayer() {
    return new CALayer();
}

UIView::UIView(Rect frame) {
    _layer = std::shared_ptr<CALayer>(initLayer());
    _layer->contentsScale = UIRenderer::main()->scale();

    _layer->delegate = this;
    setFrame(frame);
}

std::shared_ptr<UIResponder> UIView::next() {
    if (!_parentController.expired()) return _parentController.lock();
    if (!_superview.expired()) return _superview.lock();
    return nullptr;
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

void UIView::setTransform(NXAffineTransform transform) {
    _layer->setAffineTransform(transform);
}

void UIView::setMask(std::shared_ptr<UIView> mask) {
    if (_mask == mask) { return; }
    if (_mask) { _mask->removeFromSuperview(); }

    _mask = mask;
    if (mask) {
        _layer->setMask(mask->_layer);
        mask->setSuperview(shared_from_this());
    } else {
        _layer->setMask(nullptr);
    }
}

void UIView::setContentMode(UIViewContentMode mode) {
    switch (mode) {
        case UIViewContentMode::scaleToFill:
            layer()->contentsGravity = CALayerContentsGravity::resize;
            break;
        case UIViewContentMode::scaleAspectFit:
            layer()->contentsGravity = CALayerContentsGravity::resizeAspect;
            break;
        case UIViewContentMode::scaleAspectFill:
            layer()->contentsGravity = CALayerContentsGravity::resizeAspectFill;
            break;
        case UIViewContentMode::redraw:
            layer()->contentsGravity = CALayerContentsGravity::resize;
            break;
        case UIViewContentMode::center:
            layer()->contentsGravity = CALayerContentsGravity::center;
            break;
        case UIViewContentMode::top:
            layer()->contentsGravity = CALayerContentsGravity::top;
            break;
        case UIViewContentMode::bottom:
            layer()->contentsGravity = CALayerContentsGravity::bottom;
            break;
        case UIViewContentMode::left:
            layer()->contentsGravity = CALayerContentsGravity::left;
            break;
        case UIViewContentMode::right:
            layer()->contentsGravity = CALayerContentsGravity::right;
            break;
        case UIViewContentMode::topLeft:
            layer()->contentsGravity = CALayerContentsGravity::topLeft;
            break;
        case UIViewContentMode::topRight:
            layer()->contentsGravity = CALayerContentsGravity::topRight;
            break;
        case UIViewContentMode::bottomLeft:
            layer()->contentsGravity = CALayerContentsGravity::bottomLeft;
            break;
        case UIViewContentMode::bottomRight:
            layer()->contentsGravity = CALayerContentsGravity::bottomRight;
            break;
    }
}

void UIView::addSubview(std::shared_ptr<UIView> view) {
    setNeedsLayout();
    _layer->addSublayer(view->_layer);

    view->removeFromSuperview();
    _subviews.push_back(view);
    view->setSuperview(this->shared_from_this());
}

void UIView::addGestureRecognizer(std::shared_ptr<UIGestureRecognizer> gestureRecognizer) {
    gestureRecognizer->_view = weak_from_this();
    _gestureRecognizers.push_back(gestureRecognizer);
}

void UIView::setSuperview(std::shared_ptr<UIView> superview) {
    _superview = superview;
}

void UIView::insertSubviewAt(std::shared_ptr<UIView> view, int index) {
 // TODO: Need to implement
}

void UIView::removeFromSuperview() {
    auto superview = this->_superview.lock();
    if (!superview) return;

    _layer->removeFromSuperlayer();

    // If it's mask - remove
    if (superview->_mask.get() == this) {
        superview->_mask = nullptr;
    }
    else {
        superview->_subviews.erase(std::remove(superview->_subviews.begin(), superview->_subviews.end(), shared_from_this()), superview->_subviews.end());
    }
    this->setSuperview(nullptr);
    superview->setNeedsLayout();
}

// MARK: - Touch
Point UIView::convertFromView(Point point, std::shared_ptr<UIView> fromView) {
    if (!fromView) return point;
    return fromView->convertToView(point, shared_from_this());
}

Point UIView::convertToView(Point point, std::shared_ptr<UIView> toView) {
    Point selfAbsoluteOrigin;
    Point otherAbsoluteOrigin;

    std::shared_ptr<UIView> current = shared_from_this();
    while (current) {
        if (current == toView) {
            return point + selfAbsoluteOrigin;
        }
        selfAbsoluteOrigin += current->frame().origin;
        selfAbsoluteOrigin -= current->bounds().origin;
        current = current->_superview.lock();
    }

    current = toView;
    while (current) {
        otherAbsoluteOrigin += current->frame().origin;
        otherAbsoluteOrigin -= current->bounds().origin;
        current = current->_superview.lock();
    }

    Point originDifference = otherAbsoluteOrigin - selfAbsoluteOrigin;
    return point - originDifference;
}

std::shared_ptr<UIView> UIView::hitTest(Point point, UIEvent* withEvent) {
    if (isHidden() || !isUserInteractionEnabled || alpha() == 0 || !anyCurrentlyRunningAnimationsAllowUserInteraction())
        return nullptr;

    if (!this->point(point, withEvent))
        return nullptr;

    auto subviews = _subviews;
    for (int i = (int) subviews.size() - 1; i >= 0; i--) {
        Point convertedPoint = shared_from_this()->convertToView(point, subviews[i]);
        std::shared_ptr<UIView> test = subviews[i]->hitTest(convertedPoint, withEvent);
        if (test) return test;
    }

    return shared_from_this();
}

bool UIView::point(Point insidePoint, UIEvent* withEvent) {
    return bounds().contains(insidePoint);
}

// MARK: - Animations
std::set<std::shared_ptr<CALayer>> UIView::layersWithAnimations;
std::shared_ptr<CABasicAnimationPrototype> UIView::currentAnimationPrototype;

bool UIView::anyCurrentlyRunningAnimationsAllowUserInteraction() {
    if (layer()->animations.empty()) return true;

    for (auto& animation: layer()->animations) {
        auto animationGroup = animation.second->animationGroup;
        if (animationGroup && (animationGroup->options & UIViewAnimationOptions::allowUserInteraction) == UIViewAnimationOptions::allowUserInteraction) {
            return true;
        }
    }

    return false;
}

void UIView::animate(double duration, double delay, UIViewAnimationOptions options, std::function<void()> animations, std::function<void(bool)> completion) {
    auto group = std::make_shared<UIViewAnimationGroup>(options, completion);
    currentAnimationPrototype = std::make_shared<CABasicAnimationPrototype>(duration, delay, group);

    animations();

    if (currentAnimationPrototype && currentAnimationPrototype->animationGroup->queuedAnimations == 0) {
        completion(true);
    }
    
    currentAnimationPrototype = nullptr;
}


void UIView::animate(double duration, std::function<void()> animations) {
    UIView::animate( duration, 0, UIViewAnimationOptions::none, animations);
}

void UIView::animate(double duration, double delay, double damping, double initialSpringVelocity, UIViewAnimationOptions options, std::function<void()> animations, std::function<void(bool)> completion) {
    auto group = std::make_shared<UIViewAnimationGroup>(options, completion);
    currentAnimationPrototype = std::make_shared<CASpringAnimationPrototype>( duration, delay, damping, initialSpringVelocity, group);

    animations();

    if (currentAnimationPrototype && currentAnimationPrototype->animationGroup->queuedAnimations == 0) {
        completion(true);
    }
    currentAnimationPrototype = nullptr;
}

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

void UIView::display(std::shared_ptr<CALayer> layer) { }

void UIView::layoutIfNeeded() {
   if (_needsLayout) {
       layoutSubviews();
       _needsLayout = false;
   }
}

void UIView::layoutSubviews() {
    _needsLayout = false;
//    parentViewController?.viewWillLayoutSubviews()
//    parentViewController?.viewDidLayoutSubviews()
}

Size UIView::sizeThatFits(Size size) {
    return bounds().size;
}

void UIView::sizeToFit() {
    auto bounds = this->bounds();
    bounds.size = sizeThatFits(bounds.size);
    setBounds(bounds);
}

// MARK: - SDL

void UIView::sdlDrawAndLayoutTreeIfNeeded(float parentAlpha) {
    auto visibleLayer = layer()->presentationOrSelf();

    auto alpha = visibleLayer->opacity() * parentAlpha;
    if (visibleLayer->isHidden() || alpha < 0.01f) { return; }

    if (visibleLayer->_needsDisplay) {
        visibleLayer->display();
        visibleLayer->_needsDisplay = false;
    }

    if (_needsDisplay) {
        draw();
        _needsDisplay = false;
    }

    layoutIfNeeded();

    for (auto& subview: _subviews) {
        subview->sdlDrawAndLayoutTreeIfNeeded(alpha);
    }
}

}
