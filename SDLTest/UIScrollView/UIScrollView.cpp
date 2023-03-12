//
//  UIScrollView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 12.03.2023.
//

#include <UIScrollView/UIScrollView.hpp>
#include <CATransaction/CATransaction.hpp>

namespace UIKit {

UIScrollView::UIScrollView(Rect frame): UIView(frame) {
    _panGestureRecognizer = new_shared<UIPanGestureRecognizer>();
    _panGestureRecognizer->onStateChanged = [this](auto){ onPanGestureStateChanged(); };
    addGestureRecognizer(_panGestureRecognizer);
    setClipsToBounds(true);

//    applyScrollIndicatorsStyle();
//    [horizontalScrollIndicator, verticalScrollIndicator].forEach {
//        $0.alpha = 0
//        addSubview($0)
//    }
}

void UIScrollView::setContentOffset(Point offset, bool animated) {
    if (offset == contentOffset()) return;

    // Cancel deceleration animations only when contentOffset gets set without animations.
    // Otherwise we might cancel any "bounds" animations which are not iniated from velocity scrolling.
    if (_isDecelerating && UIView::currentAnimationPrototype == nullptr) {
        cancelDecelerationAnimations();
    }

    setBounds(Rect(offset, bounds().size));
    layoutScrollIndicatorsIfNeeded();

    // otherwise everything subscribing to scrollViewDidScroll is implicitly animated from velocity scroll
    CATransaction::begin();
    CATransaction::setDisableActions(!animated);
    if (!delegate.expired()) delegate.lock()->scrollViewDidScroll(shared_from_base<UIScrollView>());
    CATransaction::commit();
}

Point UIScrollView::visibleContentOffset() {
    return (layer()->presentationOrSelf())->bounds().origin;
}

Size UIScrollView::contentSize() {
    if (subviews().size() == 0) return Size();
    return subviews().front()->bounds().size;
}

Point UIScrollView::getBoundsCheckedContentOffset(Point newContentOffset) {
    auto contentSize = this->contentSize();
    auto contentHeight = fmaxf(contentSize.height, bounds().height());
    auto contentWidth = fmaxf(contentSize.width, bounds().width());
    return Point(
        fminf(fmaxf(newContentOffset.x, -contentInset.left), (contentWidth + contentInset.right) - bounds().width()),
        fminf(fmaxf(newContentOffset.y, -contentInset.top), (contentHeight + contentInset.bottom) - bounds().height())
    );
}

void UIScrollView::onPan() {
    auto translation = _panGestureRecognizer->translationInView(shared_from_this());
    _panGestureRecognizer->setTranslation(Point(), shared_from_this());

    auto panGestureVelocity = _panGestureRecognizer->velocityIn(shared_from_this());
    weightedAverageVelocity = weightedAverageVelocity * 0.2 + panGestureVelocity * 0.8;

    auto newOffset = getBoundsCheckedContentOffset(visibleContentOffset() - translation);
    setContentOffset(newOffset, false);
}

void UIScrollView::onPanGestureStateChanged() {
    switch (_panGestureRecognizer->state()) {
        case UIGestureRecognizerState::began: {
//            showScrollIndicators();
            cancelDeceleratingIfNeccessary();
            break;
        }
        case UIGestureRecognizerState::changed: {
            if (!delegate.expired()) delegate.lock()->scrollViewWillBeginDragging(shared_from_base<UIScrollView>());
            onPan();
            break;
        }
        case UIGestureRecognizerState::ended: {
            startDeceleratingIfNecessary();
            weightedAverageVelocity = Point();
            break;
        }
        // XXX: Spring back with animation:
        //case .ended, .cancelled:
        //if contentOffset.x < contentInset.left {
        //    setContentOffset(CGPoint(x: contentInset.left, y: contentOffset.y), animated: true)
        //}
        default: break;
    }
}

void UIScrollView::layoutScrollIndicatorsIfNeeded() {

}

void UIScrollView::showScrollIndicators() {

}

void UIScrollView::hideScrollIndicators() {

}

void UIScrollView::startDeceleratingIfNecessary() {
    // Only animate if instantaneous velocity is large enough
    // Otherwise we could animate after scrolling quickly, pausing for a few seconds, then letting go
    auto velocityIsLargeEnoughToDecelerate = (_panGestureRecognizer->velocityIn(shared_from_this()).magnitude() > 10);

    float dampingFactor = 0.5f; // hand-tuned
    auto nonBoundsCheckedScrollAnimationDistance = weightedAverageVelocity * dampingFactor; // hand-tuned
    auto targetOffset = getBoundsCheckedContentOffset(contentOffset() - nonBoundsCheckedScrollAnimationDistance);
    auto distanceToBoundsCheckedTarget = contentOffset() - targetOffset;

    auto willDecelerate = (velocityIsLargeEnoughToDecelerate && distanceToBoundsCheckedTarget.magnitude() > 0.0);
    if (!delegate.expired()) delegate.lock()->scrollViewDidEndDragging(shared_from_base<UIScrollView>(), willDecelerate);
    if (!willDecelerate) { hideScrollIndicators(); return; }

    // https://ariya.io/2011/10/flick-list-with-its-momentum-scrolling-and-deceleration
    // TODO: This value should be calculated from `self.decelerationRate` instead
    // But actually we want to redo this function to avoid `UIView.animate` entirely,
    // in which case we wouldn't need an animationTime at all.
    auto animationTimeConstant = 0.325f * dampingFactor;

    // This calculation is a weird approximation but it's close enough for now...
    auto animationTime = logf(distanceToBoundsCheckedTarget.magnitude()) * animationTimeConstant;

    UIViewAnimationOptions options = UIViewAnimationOptions(UIViewAnimationOptions::beginFromCurrentState | UIViewAnimationOptions::customEaseOut | UIViewAnimationOptions::allowUserInteraction);
    UIView::animate(
                    animationTime,
                    0,
                    options,
                    [this, targetOffset]() {
                        _isDecelerating = true;
                        setContentOffset(targetOffset, false);
                    },
                    [this](bool) {
                        _isDecelerating = false;
                    }
    );
}

void UIScrollView::cancelDeceleratingIfNeccessary() {
    if (!_isDecelerating) { return; }

    // Get the presentation value from the current animation
    setContentOffset(visibleContentOffset(), false);
    cancelDecelerationAnimations();
    _isDecelerating = false;
}

void UIScrollView::cancelDecelerationAnimations() {
//    if (!layer()->animations.isEmpty) {
//        layer.removeAnimation(forKey: "bounds")
//        horizontalScrollIndicator.layer.removeAnimation(forKey: "position")
//        verticalScrollIndicator.layer.removeAnimation(forKey: "position")
//    }
}

}
