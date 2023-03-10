//
//  UIView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#include <UIView/UIView.hpp>
#include <Tools/Tools.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIViewController/UIViewController.hpp>
#include <CASpringAnimationPrototype/CASpringAnimationPrototype.hpp>
#include <DispatchQueue/DispatchQueue.hpp>
#include <UINib/UINib.hpp>
#include <cxxabi.h>

namespace UIKit {

std::shared_ptr<UIView> UIView::init() {
    return new_shared<UIView>();
}

std::shared_ptr<UIView> UIView::instantiateFromXib(tinyxml2::XMLElement* element) {
    auto name = element->Name();
    auto view = UINib::xibViewsRegister[name]();
    view->yoga->setEnabled(true);
    view->applyXMLAttributes(element);

    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        auto subview = instantiateFromXib(child);
        subview->applyXMLAttributes(child);
        view->addSubview(subview);
    }

    return view;
}

void UIView::applyXMLAttributes(tinyxml2::XMLElement* element) {
    if (!element)
        return;

    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        std::string name  = attribute->Name();
        std::string value = std::string(attribute->Value());

        if (!this->applyXMLAttribute(name, value)) {
//            this->printXMLAttributeErrorMessage(element, name, value);
        }
    }
}

bool UIView::applyXMLAttribute(std::string name, std::string value) {
#define REGISTER_XIB_ATTRIBUTE(strname, parcer, setter) \
if (name == #strname) { \
auto strname = parcer(value); \
    if (!strname.has_value()) return false; \
    setter(strname.value()); \
    return true;\
}

#define REGISTER_XIB_EDGE_ATTRIBUTE(strname, parcer, setter) \
REGISTER_XIB_ATTRIBUTE(strname, parcer, setter) \
REGISTER_XIB_ATTRIBUTE(strname##Left, parcer, setter##Left) \
REGISTER_XIB_ATTRIBUTE(strname##Top, parcer, setter##Top) \
REGISTER_XIB_ATTRIBUTE(strname##Right, parcer, setter##Right) \
REGISTER_XIB_ATTRIBUTE(strname##Bottom, parcer, setter##Bottom) \
REGISTER_XIB_ATTRIBUTE(strname##Start, parcer, setter##Start) \
REGISTER_XIB_ATTRIBUTE(strname##End, parcer, setter##End) \
REGISTER_XIB_ATTRIBUTE(strname##Horizontal, parcer, setter##Horizontal) \
REGISTER_XIB_ATTRIBUTE(strname##Vertical, parcer, setter##Vertical)

    REGISTER_XIB_ATTRIBUTE(contentMode, valueToContentMode, setContentMode)
    REGISTER_XIB_ATTRIBUTE(clipsToBounds, valueToBool, setClipsToBounds)
    REGISTER_XIB_ATTRIBUTE(positionType, valueToPositionType, yoga->setPositionType)

    REGISTER_XIB_ATTRIBUTE(cornerRadius, valueToFloat, layer()->setCornerRadius)
    REGISTER_XIB_ATTRIBUTE(backgroundColor, valueToColor, setBackgroundColor)
    REGISTER_XIB_ATTRIBUTE(alpha, valueToFloat, setAlpha)

    REGISTER_XIB_ATTRIBUTE(width, valueToMetric, yoga->setWidth)
    REGISTER_XIB_ATTRIBUTE(height, valueToMetric, yoga->setHeight)
    REGISTER_XIB_ATTRIBUTE(direction, valueToDirection, yoga->setDirection)
    REGISTER_XIB_ATTRIBUTE(flexDirection, valueToFlexDirection, yoga->setFlexDirection)
    REGISTER_XIB_ATTRIBUTE(grow, valueToFloat, yoga->setFlexGrow)
    REGISTER_XIB_ATTRIBUTE(shrink, valueToFloat, yoga->setFlexShrink)
    REGISTER_XIB_ATTRIBUTE(wrap, valueToWrap, yoga->setFlexWrap)
    REGISTER_XIB_ATTRIBUTE(justifyContent, valueToJustify, yoga->setJustifyContent)
    REGISTER_XIB_ATTRIBUTE(alignItems, valueToAlign, yoga->setAlignItems)
    REGISTER_XIB_ATTRIBUTE(alignSelf, valueToAlign, yoga->setAlignSelf)
    REGISTER_XIB_ATTRIBUTE(alignContent, valueToAlign, yoga->setAlignContent)
    REGISTER_XIB_ATTRIBUTE(aspectRatio, valueToFloat, yoga->setAspectRatio)
    REGISTER_XIB_ATTRIBUTE(gap, valueToFloat, yoga->setAllGap)

    REGISTER_XIB_EDGE_ATTRIBUTE(padding, valueToMetric, yoga->setPadding)
    REGISTER_XIB_EDGE_ATTRIBUTE(margin, valueToMetric, yoga->setMargin)

    return false;
}

std::shared_ptr<CALayer> UIView::initLayer() {
    return new_shared<CALayer>();
}

UIView::UIView(Rect frame) {
    yoga = new_shared<YGLayout>(shared_from_this());
    
    _layer = initLayer();
    _layer->contentsScale = UIRenderer::main()->scale();

    _layer->delegate = this;
    setFrame(frame);
}

std::string UIView::getClassString() const {
    // Taken from: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname/4541470#4541470
    const char* name = typeid(*this).name();
    int status       = 0;

    std::unique_ptr<char, void (*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status == 0) ? res.get() : name;
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
    auto group = new_shared<UIViewAnimationGroup>(options, completion);
    currentAnimationPrototype = new_shared<CABasicAnimationPrototype>(duration, delay, group);

    animations();

    if (currentAnimationPrototype && currentAnimationPrototype->animationGroup->queuedAnimations == 0) {
        DispatchQueue::main()->async([completion]() { completion(true); });
    }
    
    currentAnimationPrototype = nullptr;
}


void UIView::animate(double duration, std::function<void()> animations) {
    UIView::animate( duration, 0, UIViewAnimationOptions::none, animations);
}

void UIView::animate(double duration, double delay, double damping, double initialSpringVelocity, UIViewAnimationOptions options, std::function<void()> animations, std::function<void(bool)> completion) {
    auto group = new_shared<UIViewAnimationGroup>(options, completion);
    currentAnimationPrototype = new_shared<CASpringAnimationPrototype>( duration, delay, damping, initialSpringVelocity, group);

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
       _needsLayout = false;
       layoutSubviews();
   }
}

void UIView::layoutSubviews() {
    _needsLayout = false;
    if (!_parentController.expired()) {
        _parentController.lock()->viewWillLayoutSubviews();
    }
    
    yoga->layoutIfNeeded();

    for (auto& subview: _subviews) {
        if (!subview->yoga->isIncludedInLayout())
            subview->setNeedsLayout();
    }

    if (!_parentController.expired()) {
        _parentController.lock()->viewDidLayoutSubviews();
    }
}

Size UIView::sizeThatFits(Size size) {
    // Apple's implementation returns current view's bounds().size
    // But in case we use Yoga's autolayout it will be better to replace it
    // with zero Size value, to allow Yoga to work properly
    return Size();
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

// MARK: - Yoga layout
void UIView::configureLayout(std::function<void(std::shared_ptr<YGLayout>)> block) {
    yoga->setEnabled(true);
    block(yoga);
}
    
}
