//
//  UIKey.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 31.03.2023.
//

#pragma once

#include <string>
#include <UIKey/UIKeyboardHIDUsage.hpp>
#include <Tools/Tools.hpp>

namespace UIKit {

enum class UIKeyModifierFlags {
    alphaShift = 1 << 0,
    shift = 1 << 1,
    control = 1 << 2,
    alternate = 1 << 3,
    command = 1 << 4,
    numericPad = 1 << 5,
};

enum class UIPressType {
    none,
    upArrow,
    downArrow,
    leftArrow,
    rightArrow,
    select,
    menu,
};

struct UIKey {
public:
    UIPressType type();
    std::string characters() { return _characters; }
    OptionSet<UIKeyModifierFlags> modifierFlags() { return _modifierFlags; }
    UIKeyboardHIDUsage keyCode() { return _keyCode; }
    
private:
    std::string _characters;
    OptionSet<UIKeyModifierFlags> _modifierFlags;
    UIKeyboardHIDUsage _keyCode;

    friend class UIApplication;
};

}
