//
//  UIView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>
#include <UIEvent/UIEvent.hpp>
#include <UIEdgeInsets/UIEdgeInsets.hpp>
#include <UIViewContentMode/UIViewContentMode.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIResponder/UIResponder.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <YogaExtensions/YogaExtensions.hpp>
#include <UIFocus/UIFocus.hpp>
#include <Tools/Tools.hpp>
#include <tinyxml2.h>
#include <functional>
#include <vector>
#include <memory>
#include <set>

namespace UIKit {

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

class UIViewController;
class UIWindow;

enum class UIViewEdgeRespects {
    none,
    layoutMargin,
    safeArea
};

class UIView: public UIResponder, public CALayerDelegate, public UIFocusItem, public enable_shared_from_this<UIView> {
public:
    static std::shared_ptr<UIView> init();
    std::string getClassString() const;

    virtual bool applyXMLAttribute(std::string name, std::string value);

    std::string tag;
    bool isUserInteractionEnabled = true;

    UIView(Rect frame);
    UIView(): UIView(Rect()) {}

    virtual std::shared_ptr<CALayer> initLayer();
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

    void setTintColor(std::optional<UIColor> tintColor);
    UIColor tintColor() const;
    virtual void tintColorDidChange();

    void setMask(std::shared_ptr<UIView> mask);
    std::shared_ptr<UIView> mask() const { return _mask; }

    void setContentMode(UIViewContentMode mode);
    UIViewContentMode contentMode() const { return _contentMode; }

    bool isTransparentTouch() { return _isTransparentTouch; }
    void setTransparentTouch(bool isTransparentTouch) { _isTransparentTouch = isTransparentTouch; }

    UIEdgeInsets layoutMargins();
    void setLayoutMargins(UIEdgeInsets layoutMargins);

    UIEdgeInsets safeAreaInsets() { return _safeAreaInsets; }

    virtual void safeAreaInsetsDidChange() {}
    virtual void layoutMarginsDidChange() {}

    bool insetsLayoutMarginsFromSafeArea() { return _insetsLayoutMarginsFromSafeArea; }
    void setInsetsLayoutMarginsFromSafeArea(bool insetsLayoutMarginsFromSafeArea);

    bool preservesSuperviewLayoutMargins() { return _preservesSuperviewLayoutMargins; }
    void setPreservesSuperviewLayoutMargins(bool preservesSuperviewLayoutMargins);

    std::vector<std::shared_ptr<UIGestureRecognizer>>* gestureRecognizers() { return &_gestureRecognizers; }
    void addGestureRecognizer(std::shared_ptr<UIGestureRecognizer> gestureRecognizer);

    virtual void addSubview(std::shared_ptr<UIView> view);
    void insertSubviewAt(std::shared_ptr<UIView> view, int index);
    void insertSubviewBelow(std::shared_ptr<UIView> view, std::shared_ptr<UIView> belowSubview);
    void removeFromSuperview();

    virtual std::shared_ptr<UIWindow> window();

    const std::vector<std::shared_ptr<UIView>>& subviews() const { return _subviews; }
    std::weak_ptr<UIView> superview() const { return _superview; }

    // Layout
    void setNeedsDisplay() { _needsDisplay = true; }
    void setNeedsLayout();// { setNeedsDisplay(); _needsLayout = true; }

    void layoutIfNeeded();
    virtual void layoutSubviews();

    virtual Size sizeThatFits(Size size);
    virtual void sizeToFit();

    // Touch
    Point convertToView(Point point, std::shared_ptr<UIView> toView);
    Point convertFromView(Point point, std::shared_ptr<UIView> fromView);
    virtual std::shared_ptr<UIView> hitTest(Point point, UIEvent* withEvent);
    virtual bool point(Point insidePoint, UIEvent* withEvent);

    // Focus
    virtual std::shared_ptr<UIFocusEnvironment> parentFocusEnvironment() override;
    virtual std::shared_ptr<UIFocusItem> getNextFocusItem(std::shared_ptr<UIView> current, UIFocusHeading focusHeading);
    virtual bool isFocused() override;

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

    // FlexLayout
    std::shared_ptr<YGLayout> yoga;
    void configureLayout(std::function<void(std::shared_ptr<YGLayout>)> block);

    UIViewEdgeRespects topEdgeRespects() { return _topEdgeRespects; }
    UIViewEdgeRespects leftEdgeRespects() { return _leftEdgeRespects; }
    UIViewEdgeRespects bottomEdgeRespects() { return _bottomEdgeRespects; }
    UIViewEdgeRespects rightEdgeRespects() { return _rightEdgeRespects; }

    void setTopEdgeRespects(UIViewEdgeRespects topEdgeRespects);
    void setLeftEdgeRespects(UIViewEdgeRespects leftEdgeRespects);
    void setBottomEdgeRespects(UIViewEdgeRespects bottomEdgeRespects);
    void setRightEdgeRespects(UIViewEdgeRespects rightEdgeRespects);

private:
    std::vector<std::shared_ptr<UIGestureRecognizer>> _gestureRecognizers;
    std::vector<std::shared_ptr<UIView>> _subviews;
    std::weak_ptr<UIView> _superview;
    std::shared_ptr<CALayer> _layer;
    std::shared_ptr<UIView> _mask;
    UIViewContentMode _contentMode;
    std::weak_ptr<UIViewController> _parentController;
    bool _isTransparentTouch = false;

    std::optional<UIColor> _tintColor;

    UIEdgeInsets _layoutMargins;
    UIEdgeInsets _calculatedLayoutMargins;
    UIEdgeInsets _safeAreaInsets;
    bool _insetsLayoutMarginsFromSafeArea = true;
    bool _preservesSuperviewLayoutMargins = false;

    void setLayoutFrame(Rect frame);
    Rect layoutFrame() const { return _layer->getLayoutFrame(); }

    void setSafeAreaInsets(UIEdgeInsets safeAreaInsets);
    void updateSafeAreaInsetsInChilds();
    void updateSafeAreaInsetsIfNeeded();
    void updateSafeAreaInsets();
    void updateLayoutMarginIfNeeded();
    void updateLayoutMargin();
    void setNeedsUpdateSafeAreaInsets() { _needsUpdateSafeAreaInsets = true; }
    void setNeedsUpdateLayoutMargins() { _needsUpdateLayoutMargins = true; }

    bool _needsLayout = true;
    bool _needsDisplay = true;
    bool _needsUpdateSafeAreaInsets = true;
    bool _needsUpdateLayoutMargins = true;

    UIViewEdgeRespects _topEdgeRespects = UIViewEdgeRespects::none;
    UIViewEdgeRespects _leftEdgeRespects = UIViewEdgeRespects::none;
    UIViewEdgeRespects _bottomEdgeRespects = UIViewEdgeRespects::none;
    UIViewEdgeRespects _rightEdgeRespects = UIViewEdgeRespects::none;
    std::shared_ptr<UIView> layoutRoot();

    float topEdgeInsetAccordingToEdgeRespection();
    float leftEdgeInsetAccordingToEdgeRespection();
    float bottomEdgeInsetAccordingToEdgeRespection();
    float rightEdgeInsetAccordingToEdgeRespection();
    UIEdgeInsets edgeInsetsAccordingToEdgeRespection();
    void updateEdgeInsets();

    void setSuperview(std::shared_ptr<UIView> superview);
    bool anyCurrentlyRunningAnimationsAllowUserInteraction();

    std::shared_ptr<UIFocusItem> searchForFocus();

    virtual void applyXMLAttributes(tinyxml2::XMLElement* element, std::map<std::string, std::shared_ptr<UIView>>* idStorage);
    static std::shared_ptr<UIView> instantiateFromXib(tinyxml2::XMLElement* element, std::map<std::string, std::shared_ptr<UIView>>* idStorage = nullptr);

    friend class UIViewController;
    friend class UIFocusSystem;
    friend class UIWindow;
    friend class UINib;
    friend class YGLayout;
};

}
