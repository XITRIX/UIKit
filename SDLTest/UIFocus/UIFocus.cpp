//
//  UIFocusEnvironment.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 02.04.2023.
//

#include <UIFocus/UIFocus.hpp>

namespace UIKit {

bool UIFocusEnvironment::shouldUpdateFocusIn(UIFocusUpdateContext context) {
    return true;
}

std::vector<std::shared_ptr<UIFocusEnvironment>> UIFocusEnvironment::preferredFocusEnvironments() {
    if (!_lastFocusEnvironment.expired()) return { _lastFocusEnvironment.lock() };
    return {};

}

void UIFocusEnvironment::didUpdateFocusIn(UIFocusUpdateContext context, UIFocusAnimationCoordinator* coordinator) {
    if (!context.nextFocusedItem().expired() && context.nextFocusedItem().lock().get() == this) {
        if (parentFocusEnvironment()) {
            parentFocusEnvironment()->_lastFocusEnvironment = context.nextFocusedItem();
        }
    }
    
    if (parentFocusEnvironment()) parentFocusEnvironment()->didUpdateFocusIn(context, coordinator);
}

}
