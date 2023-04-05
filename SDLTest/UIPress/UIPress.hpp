//
//  UIPress.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 30.03.2023.
//

#pragma once

#include <Tools/Tools.hpp>
#include <UIKey/UIKey.hpp>
#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <time.h>
#include <optional>

namespace UIKit {

enum class UIPressPhase {
    began, changed, stationary, ended
};

class UIResponder;
class UIWindow;
class UIPress {
public:
    UIPress(Time timestamp = getCPUTimeUsec());

    Time timestamp() const { return _timestamp; }
    UIPressPhase phase() const { return _phase; }

    std::weak_ptr<UIWindow> window() const { return _window; }
    std::weak_ptr<UIResponder> responder() const { return _responder; }

    std::vector<std::weak_ptr<UIGestureRecognizer>> gestureRecognizers() { return _gestureRecognizers; };
    std::optional<UIKey> key() { return _key; }

private:
    std::weak_ptr<UIResponder> _responder;
    std::weak_ptr<UIWindow> _window;
    
    UIPressPhase _phase = UIPressPhase::began;
    Time _timestamp;

    std::vector<std::weak_ptr<UIGestureRecognizer>> _gestureRecognizers;
    bool _hasBeenCancelledByAGestureRecognizer = false;
    std::optional<UIKey> _key;

    void setForWindow(std::shared_ptr<UIWindow> window);
    void runPressActionOnRecognizerHierachy(std::function<void(std::shared_ptr<UIGestureRecognizer>)> action);
    bool hasBeenCancelledByAGestureRecognizer() const { return _hasBeenCancelledByAGestureRecognizer; }

    friend class UIApplication;
    friend class UIGestureRecognizer;
    friend class UIWindow;
};

}
