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
    UIView::didUpdateFocusIn(context, coordinator);
    
    if (context.nextFocusedItem().lock() == shared_from_this()) {
        coordinator->addCoordinatedAnimations([this]() {
            setBackgroundColor(UIColor::systemTint);
            setTransform(NXAffineTransform::scale(1.0f));
        });
    } else {
        coordinator->addCoordinatedAnimations([this]() {
            setBackgroundColor(UIColor::clear);
            setTransform(NXAffineTransform::identity);
        });
    }
}


}

