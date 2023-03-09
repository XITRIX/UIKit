//
//  UITouch.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#pragma once

#include <Utils/Utils.hpp>
#include <Geometry/Geometry.hpp>
#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <vector>

namespace UIKit {

enum class UITouchPhase {
    began, moved, ended
};

class UIView;
class UIWindow;
class UITouch {
public:
    UITouch(): UITouch(0, Point(), getCPUTimeUsec()) {}
    UITouch(unsigned long touchId, Point point, Time timestamp);

    unsigned long touchId() const { return _touchId; }

    std::weak_ptr<UIView> view() const { return _view; }
    std::weak_ptr<UIWindow> window() const { return _window; }

    UITouchPhase phase() const { return _phase; }
    std::time_t timestamp() const { return _timestamp; }

    Point absoluteLocation() const { return _absoluteLocation; }
    Point previousAbsoluteLocation() const { return _previousAbsoluteLocation; }

    std::vector<std::weak_ptr<UIGestureRecognizer>> gestureRecognizers() { return _gestureRecognizers; };

    void updateAbsoluteLocation(Point newLocation);

    Point locationIn(std::shared_ptr<UIView> view);
    Point previousLocationIn(std::shared_ptr<UIView> view);

    void runTouchActionOnRecognizerHierachy(std::function<void(std::shared_ptr<UIGestureRecognizer>)> action);

    bool hasBeenCancelledByAGestureRecognizer() const { return _hasBeenCancelledByAGestureRecognizer; }

private:
    unsigned long _touchId;

    std::weak_ptr<UIView> _view;
    std::weak_ptr<UIWindow> _window;

    UITouchPhase _phase = UITouchPhase::began;
    std::time_t _timestamp;

    Point _absoluteLocation;
    Point _previousAbsoluteLocation;

    std::vector<std::weak_ptr<UIGestureRecognizer>> _gestureRecognizers;
    bool _hasBeenCancelledByAGestureRecognizer = false;

    friend class UIApplication;
    friend class UIWindow;
};

constexpr bool operator==(const UITouch& lhs, const UITouch& rhs) {
    return lhs.touchId() == rhs.touchId();
}

}
