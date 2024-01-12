//
//  UIApplicationDelegate.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationDelegate/UIApplicationDelegate.hpp>
#include <UIKit.hpp>

namespace UIKit {

void UIApplicationDelegate::applicationNeedsXIBRegistration(UIApplication* application) {
    REGISTER_NIB(UIView)
    REGISTER_NIB(UILabel)
    REGISTER_NIB(UIControl)
    REGISTER_NIB(UIImageView)
    REGISTER_NIB(UIScrollView)
    REGISTER_NIB(UIBlurView)
}

}
