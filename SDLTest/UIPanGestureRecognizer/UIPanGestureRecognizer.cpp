//
//  UIPanGestureRecognizer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.03.2023.
//

#include <UIPanGestureRecognizer/UIPanGestureRecognizer.hpp>
#include <UITouch/UITouch.hpp>
#include <UIWindow/UIWindow.hpp>

namespace UIKit {

#define THRESHOLD 10

Point UIPanGestureRecognizer::translationInView(std::shared_ptr<UIView> view) {
    if (!trackingTouch) return Point();

    auto positionInTargetView = trackingTouch->locationIn(view);
    Point initialPositionInTargetView;
    if (trackingTouch->window().expired()) initialPositionInTargetView = initialTouchPoint;
    else { initialPositionInTargetView = view ? trackingTouch->window().lock()->convertToView(initialTouchPoint, view) : initialTouchPoint; }

    return positionInTargetView - initialPositionInTargetView;
}

void UIPanGestureRecognizer::setTranslation(Point translation, std::shared_ptr<UIView> inView) {
    if (!trackingTouch) return;

    auto positionInTargetView = trackingTouch->locationIn(nullptr);
    initialTouchPoint = positionInTargetView - translation;
}

Point UIPanGestureRecognizer::velocityIn(std::shared_ptr<UIView> view, float timeDiffSeconds) {
    if (!trackingTouch || timeDiffSeconds == 0) return Point();

    Point curPos = trackingTouch->locationIn(view);
    Point prevPos = trackingTouch->previousLocationIn(view);

    return (curPos - prevPos) / (float)timeDiffSeconds;
}

Point UIPanGestureRecognizer::velocityIn(std::shared_ptr<UIView> view) {
    float timeDiffSeconds = touchesMovedTimestamp - previousTouchesMovedTimestamp;
    timeDiffSeconds /= 100000.0f;
    return velocityIn(view, timeDiffSeconds);
}

void UIPanGestureRecognizer::touchesBegan(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    UIGestureRecognizer::touchesBegan(touches, event);
    if (!trackingTouch) {
        trackingTouch = touches[0];
        initialTouchPoint = trackingTouch->locationIn(nullptr);
        touchesMovedTimestamp = trackingTouch->timestamp();
        previousTouchesMovedTimestamp = trackingTouch->timestamp();
        onStateChanged(UIGestureRecognizerState::possible);
    }
}

void UIPanGestureRecognizer::touchesMoved(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    UIGestureRecognizer::touchesMoved(touches, event);
    if (trackingTouch == touches[0]) {
        if (state() == UIGestureRecognizerState::possible) {
            Point diff = initialTouchPoint - trackingTouch->locationIn(nullptr);
//            printf("x %f, y %f\n", diff.x, diff.y);
            if (abs(diff.x) >= THRESHOLD || abs(diff.y) >= THRESHOLD) {
                previousTouchesMovedTimestamp = touchesMovedTimestamp;
                touchesMovedTimestamp = trackingTouch->timestamp();
                setState(UIGestureRecognizerState::began);
                setState(UIGestureRecognizerState::changed);
            }
        } else if (state() == UIGestureRecognizerState::changed) {
            previousTouchesMovedTimestamp = touchesMovedTimestamp;
            touchesMovedTimestamp = trackingTouch->timestamp();
            onStateChanged(UIGestureRecognizerState::changed);
        }
    }
}

void UIPanGestureRecognizer::touchesEnded(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    UIGestureRecognizer::touchesEnded(touches, event);
    if (trackingTouch == touches[0]) {
        previousTouchesMovedTimestamp = touchesMovedTimestamp;
        touchesMovedTimestamp = trackingTouch->timestamp();
        setState(UIGestureRecognizerState::ended);
        trackingTouch = nullptr;
    }
}

}
