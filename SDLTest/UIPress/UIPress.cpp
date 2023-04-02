//
//  UIPress.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 30.03.2023.
//

#include <UIPress/UIPress.hpp>
#include <UIWindow/UIWindow.hpp>

namespace UIKit {

UIPress::UIPress(Time timestamp):
    _timestamp(timestamp)
{ }

void UIPress::setForWindow(std::shared_ptr<UIWindow> window) {
    _window = window;

    if (!window || !window->rootViewController()) {
        _responder = window;
        return;
    }

    auto vc = window->rootViewController();
    while (!vc->children().empty()) {
        vc = vc->children().front();
    }
    
    _responder = vc->view();
}

}
