//
//  UIApplicationDelegate.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationDelegate/UIApplicationDelegate.hpp>
#include <TestViewController/TestViewController.hpp>
#include <YogaTestViewController/YogaTestViewController.hpp>
#include <NavigationViewController/NavigationViewController.hpp>
#include <TextViewController/TextViewController.hpp>

namespace UIKit {

bool UIApplicationDelegate::applicationDidFinishLaunchingWithOptions(UIApplication* application, std::map<std::string, std::any> launchOptions) {
    window = new_shared<UIWindow>();

    auto vc = new_shared<TestViewController>();
//    auto vc = new_shared<YogaTestViewController>();
//    auto vc = new_shared<NavigationViewController>();
//    auto vc = new_shared<TextViewController>();
    window->setRootViewController(vc);
    window->makeKeyAndVisible();
    return true;
}

}
