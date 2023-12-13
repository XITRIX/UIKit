//
//  UITouch.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#include <UITouch/UITouch.hpp>
#include <UIWindow/UIWindow.hpp>
#include <utility>

namespace UIKit {

UITouch::UITouch(unsigned long touchId, Point atPoint, Time timestamp):
_touchId(touchId),
_timestamp(timestamp),
_absoluteLocation(atPoint),
_previousAbsoluteLocation(atPoint)
{ }

void UITouch::updateAbsoluteLocation(Point newLocation) {
    _previousAbsoluteLocation = _absoluteLocation;
    _absoluteLocation = newLocation;
}

Point UITouch::locationIn(std::shared_ptr<UIView> view) {
    if (_window.expired()) return _absoluteLocation;
    return _window.lock()->convertToView(_absoluteLocation, std::move(view));
}

Point UITouch::previousLocationIn(std::shared_ptr<UIView> view) {
    if (_window.expired()) return _previousAbsoluteLocation;
    return _window.lock()->convertToView(_previousAbsoluteLocation, std::move(view));
}

void UITouch::runTouchActionOnRecognizerHierachy(const std::function<void(std::shared_ptr<UIGestureRecognizer>)>& action) {
    for (const auto& recognizer: _gestureRecognizers) {
        if (_hasBeenCancelledByAGestureRecognizer) return;
        if (recognizer.expired() || !recognizer.lock()->isEnabled()) continue;
        action(recognizer.lock());
    }
}


}
