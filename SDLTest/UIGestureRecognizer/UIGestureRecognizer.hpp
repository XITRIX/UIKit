//
//  UIGestureRecognizer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <Tools/Tools.hpp>

namespace UIKit {

class UITouch;

enum class UIGestureRecognizerState {
    possible,
    began,
    recognized,
    changed,
    ended,
    cancelled,
    failed
};

class UIView;
class UIEvent;
class UIGestureRecognizer;
class UIGestureRecognizerDelegate {
public:
    virtual bool gestureRecognizerShouldRecognizeSimultaneouslyWith(std::shared_ptr<UIGestureRecognizer> gestureRecognizer, std::shared_ptr<UIGestureRecognizer> otherGestureRecognizer) { return false; }
};

class UIGestureRecognizer: public enable_shared_from_this<UIGestureRecognizer> {
public:
    std::weak_ptr<UIGestureRecognizerDelegate> delegate;
    std::function<void(UIGestureRecognizerState)> onStateChanged = [](auto state){};

    UIGestureRecognizer(std::function<void(UIGestureRecognizerState)> onStateChanged = [](auto) {});
    virtual ~UIGestureRecognizer();

    bool isEnabled() { return _isEnabled; }
    void setEnabled(bool enabled);

    UIGestureRecognizerState state() { return _state; }
    void setState(UIGestureRecognizerState state);

    std::weak_ptr<UIView> view() { return _view; }

    virtual void touchesBegan(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesMoved(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesEnded(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesCancelled(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);

private:
    bool _isEnabled = true;
    std::weak_ptr<UIView> _view;
    std::vector<std::shared_ptr<UITouch>> _allTouches;
    UIGestureRecognizerState _state = UIGestureRecognizerState::possible;

    void _touchesBegan(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    void _touchesMoved(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    void _touchesEnded(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    void _touchesCancelled(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);

    bool recognitionCondition();

    void addTouch(std::shared_ptr<UITouch> touch);
    void removeTouch(std::shared_ptr<UITouch> touch);
    void cancelOtherGestureRecognizersThatShouldNotRecognizeSimultaneously();

    friend class UIView;
};

}
