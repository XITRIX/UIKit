//
//  UIKey.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 31.03.2023.
//

#include <UIKey/UIKey.hpp>

namespace UIKit {

UIPressType UIKey::type() {
    if (_keyCode == UIKeyboardHIDUsage::keyboardUpArrow) {
        return UIPressType::upArrow;
    }
    if (_keyCode == UIKeyboardHIDUsage::keyboardLeftAlt) {
        return UIPressType::leftArrow;
    }
    if (_keyCode == UIKeyboardHIDUsage::keyboardDownArrow) {
        return UIPressType::downArrow;
    }
    if (_keyCode == UIKeyboardHIDUsage::keyboardRightArrow) {
        return UIPressType::rightArrow;
    }
    if (_keyCode == UIKeyboardHIDUsage::keyboardReturnOrEnter) {
        return UIPressType::select;
    }
    return UIPressType::none;
}

}
