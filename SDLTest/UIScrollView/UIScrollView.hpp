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

class UIScrollView: public UIView {
public:
    std::weak_ptr<UIScrollViewDelegate> delegate;

    UIScrollView(Rect frame = Rect());

    Point contentOffset() { return bounds().origin; }
    void setContentOffset(Point offset, bool animated);

    Size contentSize();
//    void setContentSize(Size size) { _contentSize = size; }
private:
    std::shared_ptr<UIPanGestureRecognizer> _panGestureRecognizer;
    bool _isDecelerating = false;
    Point weightedAverageVelocity;

    UIEdgeInsets contentInset;
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
