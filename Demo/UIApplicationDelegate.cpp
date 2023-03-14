//
//  UIApplicationDelegate.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationDelegate/UIApplicationDelegate.hpp>
#include <TestViewController/TestViewController.hpp>
#include <YogaTestViewController/YogaTestViewController.hpp>
#include <IBTestViewController/IBTestViewController.hpp>

namespace UIKit {

bool UIApplicationDelegate::applicationDidFinishLaunchingWithOptions(UIApplication* application, std::map<std::string, std::any> launchOptions) {
    window = new_shared<UIWindow>();

//    auto vc = new_shared<TestViewController>();
//    auto vc = new_shared<YogaTestViewController>();
    auto vc = new_shared<IBTestViewController>();
    window->setRootViewController(vc);

    window->makeKeyAndVisible();
    return true;
}

}
