//
//  UIApplicationDelegate.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationDelegate/UIApplicationDelegate.hpp>
#include <TestViewController/TestViewController.hpp>

namespace UIKit {

bool UIApplicationDelegate::applicationDidFinishLaunchingWithOptions(UIApplication* application, std::map<std::string, std::any> launchOptions) {
    window = std::make_shared<UIWindow>();

    auto vc = std::make_shared<TestViewController>();
    window->setRootViewController(vc);

    window->makeKeyAndVisible();
    return true;
}

}
