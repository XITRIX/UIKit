//
//  UIViewAnimationOptions.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

namespace UIKit {

enum UIViewAnimationOptions {
    allowUserInteraction = 1 << 0,
    beginFromCurrentState = 1 << 1,
    curveEaseIn = 1 << 2,
    curveEaseOut = 1 << 3,
    curveEaseInOut = 1 << 4,
    curveLinear = 1 << 5,
    customEaseOut = 1 << 9
};

}
