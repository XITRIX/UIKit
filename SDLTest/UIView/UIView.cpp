//
//  UIView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#include <UIView/UIView.hpp>
#include <Tools/Tools.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIWindow/UIWindow.hpp>
#include <UIViewController/UIViewController.hpp>
#include <CASpringAnimationPrototype/CASpringAnimationPrototype.hpp>
#include <DispatchQueue/DispatchQueue.hpp>
#include <UINib/UINib.hpp>
#include <cxxabi.h>

namespace UIKit {

std::shared_ptr<UIView> UIView::init() {
    return new_shared<UIView>();
}

std::shared_ptr<UIView> UIView::instantiateFromXib(tinyxml2::XMLElement* element, std::map<std::string, std::shared_ptr<UIView>>* idStorage) {
    auto name = element->Name();
    auto view = UINib::xibViewsRegister[name]();
    view->applyXMLAttributes(element, idStorage);

    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        auto subview = instantiateFromXib(child, idStorage);
        view->addSubview(subview);
    }

    return view;
}

void UIView::applyXMLAttributes(tinyxml2::XMLElement* element, std::map<std::string, std::shared_ptr<UIView>>* idStorage) {
    if (!element)
        return;

    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        std::string name  = attribute->Name();
        std::string value = std::string(attribute->Value());

        if (name == "id") {
            tag = value;
            if (!idStorage) continue;
            idStorage->insert(std::pair<std::string, std::shared_ptr<UIView>>(value, shared_from_this()));
            continue;
        }

        if (!this->applyXMLAttribute(name, value)) {
            printf("Error XML attribute parsing Name: %s with Value%s\n", name.c_str(), value.c_str());
            //            this->printXMLAttributeErrorMessage(element, name, value);
        }
    }
}

bool UIView::applyXMLAttribute(std::string name, std::string value) {
    REGISTER_XIB_ATTRIBUTE(contentMode, valueToContentMode, setContentMode)
    REGISTER_XIB_ATTRIBUTE(clipsToBounds, valueToBool, setClipsToBounds)
    REGISTER_XIB_ATTRIBUTE(positionType, valueToPositionType, yoga->setPositionType)
    REGISTER_XIB_ATTRIBUTE(isTransparentTouch, valueToBool, setTransparentTouch)
    REGISTER_XIB_ATTRIBUTE(preservesSuperviewLayoutMargins, valueToBool, setPreservesSuperviewLayoutMargins)

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

    REGISTER_XIB_ATTRIBUTE(topEdgeRespects, valueToEdgeRespects, setTopEdgeRespects)
    REGISTER_XIB_ATTRIBUTE(leftEdgeRespects, valueToEdgeRespects, setLeftEdgeRespects)
    REGISTER_XIB_ATTRIBUTE(rightEdgeRespects, valueToEdgeRespects, setRightEdgeRespects)
    REGISTER_XIB_ATTRIBUTE(bottomEdgeRespects, valueToEdgeRespects, setBottomEdgeRespects)

    return false;
}

std::shared_ptr<CALayer> UIView::initLayer() {
    return new_shared<CALayer>();
}

UIView::UIView(Rect frame) {
    yoga = new_shared<YGLayout>(shared_from_this());
    yoga->setEnabled(true);
    
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
    setNeedsUpdateSafeAreaInsets();
}

void UIView::setLayoutFrame(Rect frame) {
    if (this->layoutFrame().size != frame.size) {
        setNeedsLayout();
    }
    _layer->setLayoutFrame(frame);
    setNeedsUpdateSafeAreaInsets();
}

void UIView::setBounds(Rect bounds) {
    if (this->bounds().size != bounds.size) {
        setNeedsLayout();
        setNeedsUpdateSafeAreaInsets();
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

void UIView::setInsetsLayoutMarginsFromSafeArea(bool insetsLayoutMarginsFromSafeArea) {
    if (_insetsLayoutMarginsFromSafeArea == insetsLayoutMarginsFromSafeArea) return;
    _insetsLayoutMarginsFromSafeArea = insetsLayoutMarginsFromSafeArea;
    setNeedsLayout();
}

void UIView::setPreservesSuperviewLayoutMargins(bool preservesSuperviewLayoutMargins) {
    if (_preservesSuperviewLayoutMargins == preservesSuperviewLayoutMargins) return;
    _preservesSuperviewLayoutMargins = preservesSuperviewLayoutMargins;
    setNeedsLayout();
}

UIEdgeInsets UIView::layoutMargins() {
    return _calculatedLayoutMargins;
}

void UIView::setLayoutMargins(UIEdgeInsets layoutMargins) {
    if (_layoutMargins == layoutMargins) return;
    _layoutMargins = layoutMargins;
    setNeedsUpdateLayoutMargins();
    setNeedsLayout();
}

void UIView::setSafeAreaInsets(UIEdgeInsets safeAreaInsets) {
    if (_safeAreaInsets == safeAreaInsets) return;
    _safeAreaInsets = safeAreaInsets;
    setNeedsUpdateLayoutMargins();
    updateSafeAreaInsetsInChilds();
    safeAreaInsetsDidChange();
    setNeedsLayout();
}

void UIView::updateSafeAreaInsetsInChilds() {
    for (auto& subview: _subviews) {
        subview->setNeedsUpdateSafeAreaInsets();
    }
}

void UIView::updateSafeAreaInsetsIfNeeded() {
    if (_needsUpdateSafeAreaInsets) {
        _needsUpdateSafeAreaInsets = false;
        updateSafeAreaInsets();
    }
}

void UIView::updateSafeAreaInsets() {
    if (_superview.expired()) {
        if (shared_from_base<UIWindow>() != nullptr) return;
        else return setSafeAreaInsets(UIEdgeInsets::zero);
    }

    auto parentSafeArea = _superview.lock()->_safeAreaInsets;
    auto parentSize = _superview.lock()->bounds().size;

    layoutIfNeeded();
    auto frame = this->frame();


    auto newSafeArea = UIEdgeInsets(fmaxf(0, parentSafeArea.top - fmaxf(0, frame.minY())),
                                    fmaxf(0, parentSafeArea.left - fmaxf(0, frame.minX())),
                                    fmaxf(0, parentSafeArea.bottom - fmaxf(0, (parentSize.height - frame.maxY()))),
                                    fmaxf(0, parentSafeArea.right - fmaxf(0, (parentSize.width - frame.maxX()))));

    if (!_parentController.expired()) {
        newSafeArea += _parentController.lock()->additionalSafeAreaInsets();
    }

    setSafeAreaInsets(newSafeArea);
}

void UIView::updateLayoutMarginIfNeeded() {
    if (_needsUpdateLayoutMargins) {
        _needsUpdateLayoutMargins = false;
        updateLayoutMargin();
    }
}

void UIView::updateLayoutMargin() {
    auto margins = _layoutMargins;

    bool needsSuperviewMargins = _preservesSuperviewLayoutMargins && !superview().expired();
    if (needsSuperviewMargins && _insetsLayoutMarginsFromSafeArea) {
        auto superviewMargins = superview().lock()->layoutMargins();
        auto maxCombination = UIEdgeInsets(fmaxf(_safeAreaInsets.top, superviewMargins.top),
                                           fmaxf(_safeAreaInsets.left, superviewMargins.left),
                                           fmaxf(_safeAreaInsets.bottom, superviewMargins.bottom),
                                           fmaxf(_safeAreaInsets.right, superviewMargins.right));
        margins += maxCombination;
    } else {
        if (_insetsLayoutMarginsFromSafeArea) {
            margins += _safeAreaInsets;
        }

        if (needsSuperviewMargins) {
            margins += superview().lock()->layoutMargins();
        }
    }

    if (!_parentController.expired() && _parentController.lock()->viewRespectsSystemMinimumLayoutMargins()) {
        auto minMargins = _parentController.lock()->systemMinimumLayoutMargins();
        margins = UIEdgeInsets(fmaxf(margins.top, minMargins.top),
                               fmaxf(margins.left, minMargins.left),
                               fmaxf(margins.bottom, minMargins.bottom),
                               fmaxf(margins.right, minMargins.right));
    }

    if (_calculatedLayoutMargins != margins) {
        _calculatedLayoutMargins = margins;
        layoutMarginsDidChange();
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
    bool needToNotifyViewController = false;
    if (!view->_parentController.expired()) {
        auto window = this->window();
        if (window) {
            needToNotifyViewController = true;
        }
    }

    setNeedsLayout();
    view->removeFromSuperview();

    if (needToNotifyViewController)
        view->_parentController.lock()->viewWillAppear(true);
    
    _layer->addSublayer(view->_layer);
    _subviews.push_back(view);
    view->setSuperview(this->shared_from_this());
    view->setNeedsUpdateSafeAreaInsets();
}

void UIView::addGestureRecognizer(std::shared_ptr<UIGestureRecognizer> gestureRecognizer) {
    gestureRecognizer->_view = weak_from_this();
    _gestureRecognizers.push_back(gestureRecognizer);
}

std::shared_ptr<UIWindow> UIView::window() {
    if (!_superview.expired()) return _superview.lock()->window();
    return nullptr;
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

std::shared_ptr<UIView> UIView::layoutRoot() {
    auto view = shared_from_this();
    while (view && !view->yoga->isRoot()) {
        view = view->superview().lock();
    }
    return view;
}

void UIView::setNeedsLayout() {
    setNeedsDisplay();
    // Needs to recalculate Yoga from root
    auto layoutRoot = this->layoutRoot();
    if (layoutRoot) layoutRoot->_needsLayout = true;
    _needsLayout = true;
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

    if (_isTransparentTouch && (backgroundColor() == std::nullopt || backgroundColor() == UIColor::clear))
        return nullptr;

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

    updateEdgeInsets();
    yoga->layoutIfNeeded();
    updateSafeAreaInsets();

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

    updateSafeAreaInsetsIfNeeded();
    updateLayoutMarginIfNeeded();
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

void UIView::updateEdgeInsets() {
    yoga->setPaddingTop(YGValue { topEdgeInsetAccordingToEdgeRespection(), YGUnitPoint });
    yoga->setPaddingLeft(YGValue { leftEdgeInsetAccordingToEdgeRespection(), YGUnitPoint });
    yoga->setPaddingBottom(YGValue { bottomEdgeInsetAccordingToEdgeRespection(), YGUnitPoint });
    yoga->setPaddingRight(YGValue { rightEdgeInsetAccordingToEdgeRespection(), YGUnitPoint });
}

void UIView::setTopEdgeRespects(UIViewEdgeRespects topEdgeRespects) {
    if (_topEdgeRespects == topEdgeRespects) return;
    _topEdgeRespects = topEdgeRespects;
    setNeedsLayout();
}

void UIView::setLeftEdgeRespects(UIViewEdgeRespects leftEdgeRespects) {
    if (_leftEdgeRespects == leftEdgeRespects) return;
    _leftEdgeRespects = leftEdgeRespects;
    setNeedsLayout();
}

void UIView::setBottomEdgeRespects(UIViewEdgeRespects bottomEdgeRespects) {
    if (_bottomEdgeRespects == bottomEdgeRespects) return;
    _bottomEdgeRespects = bottomEdgeRespects;
    setNeedsLayout();
}

void UIView::setRightEdgeRespects(UIViewEdgeRespects rightEdgeRespects) {
    if (_rightEdgeRespects == rightEdgeRespects) return;
    _rightEdgeRespects = rightEdgeRespects;
    setNeedsLayout();
}

float UIView::topEdgeInsetAccordingToEdgeRespection() {
    switch (_topEdgeRespects) {
        case UIViewEdgeRespects::layoutMargin:
            return layoutMargins().top;
        case UIViewEdgeRespects::safeArea:
            return safeAreaInsets().top;
        case UIViewEdgeRespects::none:
            return 0;
    }
}

float UIView::leftEdgeInsetAccordingToEdgeRespection() {
    switch (_leftEdgeRespects) {
        case UIViewEdgeRespects::layoutMargin:
            return layoutMargins().left;
        case UIViewEdgeRespects::safeArea:
            return safeAreaInsets().left;
        case UIViewEdgeRespects::none:
            return 0;
    }
}

float UIView::bottomEdgeInsetAccordingToEdgeRespection() {
    switch (_bottomEdgeRespects) {
        case UIViewEdgeRespects::layoutMargin:
            return layoutMargins().bottom;
        case UIViewEdgeRespects::safeArea:
            return safeAreaInsets().bottom;
        case UIViewEdgeRespects::none:
            return 0;
    }
}

float UIView::rightEdgeInsetAccordingToEdgeRespection() {
    switch (_rightEdgeRespects) {
        case UIViewEdgeRespects::layoutMargin:
            return layoutMargins().right;
        case UIViewEdgeRespects::safeArea:
            return safeAreaInsets().right;
        case UIViewEdgeRespects::none:
            return 0;
    }
}

UIEdgeInsets UIView::edgeInsetsAccordingToEdgeRespection() {
    return UIEdgeInsets(topEdgeInsetAccordingToEdgeRespection(),
                        leftEdgeInsetAccordingToEdgeRespection(),
                        bottomEdgeInsetAccordingToEdgeRespection(),
                        rightEdgeInsetAccordingToEdgeRespection());
}
    
}
