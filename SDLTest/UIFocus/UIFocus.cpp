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

void UIFocusEnvironment::didUpdateFocusIn(UIFocusUpdateContext context, UIFocusAnimationCoordinator coordinator) { }

}
