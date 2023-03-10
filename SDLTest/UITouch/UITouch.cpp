//
//  UITouch.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#include <UITouch/UITouch.hpp>
#include <UIWindow/UIWindow.hpp>

namespace UIKit {

UITouch::UITouch(unsigned long touchId, Point atPoint, Time timestamp):
_touchId(touchId),
_absoluteLocation(atPoint),
_previousAbsoluteLocation(atPoint),
_timestamp(timestamp)
{ }

void UITouch::updateAbsoluteLocation(Point newLocation) {
    _previousAbsoluteLocation = _absoluteLocation;
    _absoluteLocation = newLocation;
}

Point UITouch::locationIn(std::shared_ptr<UIView> view) {
    if (_window.expired()) return _absoluteLocation;
    return _window.lock()->convertToView(_absoluteLocation, view);
}

Point UITouch::previousLocationIn(std::shared_ptr<UIView> view) {
    if (_window.expired()) return _previousAbsoluteLocation;
    return _window.lock()->convertToView(_previousAbsoluteLocation, view);
}

void UITouch::runTouchActionOnRecognizerHierachy(std::function<void(std::shared_ptr<UIGestureRecognizer>)> action) {
    for (auto recognizer: _gestureRecognizers) {
        if (_hasBeenCancelledByAGestureRecognizer) return;
        if (recognizer.expired() || !recognizer.lock()->isEnabled()) continue;
        action(recognizer.lock());
    }
}


}
