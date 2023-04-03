//
//  UIControl.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 03.04.2023.
//

#include <UIControl/UIControl.hpp>

namespace UIKit {

bool UIControl::canBecomeFocused() {
    return true;
}

void UIControl::didUpdateFocusIn(UIFocusUpdateContext context, UIFocusAnimationCoordinator* coordinator) {
    if (context.nextFocusedItem().lock() == shared_from_this()) {
        coordinator->addCoordinatedAnimations([this]() {
            setTransform(NXAffineTransform::scale(1.2f));
        });
    } else {
        coordinator->addCoordinatedAnimations([this]() {
            setTransform(NXAffineTransform::identity);
        });
    }
}


}

