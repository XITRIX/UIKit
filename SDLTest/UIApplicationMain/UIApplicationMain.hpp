//
//  UIApplicationMain.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once
#include <memory>
#include <UIApplication/UIApplication.hpp>

namespace UIKit {

class UIApplicationDelegate;
int UIApplicationMain(std::shared_ptr<UIApplicationDelegate> appDelegate = std::make_shared<UIApplicationDelegate>());

}
