//
//  UIScrollView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 12.03.2023.
//

#pragma once

#include <UIView/UIView.hpp>
#include <UIPanGestureRecognizer/UIPanGestureRecognizer.hpp>

namespace UIKit {

class UIScrollView;
class UIScrollViewDelegate {
public:
    virtual void scrollViewWillBeginDragging(std::shared_ptr<UIScrollView> scrollView) {}
    virtual void scrollViewDidScroll(std::shared_ptr<UIScrollView> scrollView) {}
    virtual void scrollViewDidEndDragging(std::shared_ptr<UIScrollView> scrollView, bool willDecelerate) {}
};

enum class UIScrollViewContentInsetAdjustmentBehavior {
    automatic,
    scrollableAxes,
    never,
    always
};

class UIScrollView: public UIView {
public:
    static std::shared_ptr<UIScrollView> init();

    std::weak_ptr<UIScrollViewDelegate> delegate;

    UIScrollView(Rect frame = Rect());

    virtual void addSubview(std::shared_ptr<UIView> view) override;
    bool applyXMLAttribute(std::string name, std::string value) override;

    void layoutMarginsDidChange() override;

    Point contentOffset() { return bounds().origin; }
    void setContentOffset(Point offset, bool animated);

    UIEdgeInsets contentInset() { return _contentInset; }
    void setContentInset(UIEdgeInsets contentInset) { _contentInset = contentInset; }

    bool bounceHorizontally() { return _bounceHorizontally; }
    void setBounceHorizontally(bool bounceHorizontally);

    bool bounceVertically() { return _bounceVertically; }
    void setBounceVertically(bool bounceVertically);

    UIScrollViewContentInsetAdjustmentBehavior contentInsetAdjustmentBehavior() { return _contentInsetAdjustmentBehavior; }
    void setContentInsetAdjustmentBehavior(UIScrollViewContentInsetAdjustmentBehavior contentInsetAdjustmentBehavior);

    Size contentSize();
//    void setContentSize(Size size) { _contentSize = size; }

private:
    std::shared_ptr<UIPanGestureRecognizer> _panGestureRecognizer;
    bool _isDecelerating = false;
    Point weightedAverageVelocity;

    bool _bounceHorizontally = false;
    bool _bounceVertically = false;
    UIScrollViewContentInsetAdjustmentBehavior _contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentBehavior::automatic;

    UIEdgeInsets _lastLayoutMargins;
    UIEdgeInsets _contentInset;
//    Size _contentSize;

    void onPan();
    void onPanGestureStateChanged();

    Point visibleContentOffset();
    Point getBoundsCheckedContentOffset(Point newContentOffset);

    void layoutScrollIndicatorsIfNeeded();
    void showScrollIndicators();
    void hideScrollIndicators();

    void startDeceleratingIfNecessary();
    void cancelDeceleratingIfNeccessary();
    void cancelDecelerationAnimations();
};

}
