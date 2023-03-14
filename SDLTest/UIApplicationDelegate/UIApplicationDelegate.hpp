//
//  UIApplicationDelegate.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIWindow/UIWindow.hpp>

#define REGISTER_NIB(n) UINib::registerXIB(#n, n::init);

namespace UIKit {

class UIApplication;
class UIApplicationDelegate {
public:
    std::shared_ptr<UIWindow> window;

    virtual bool applicationDidFinishLaunchingWithOptions(UIApplication* application, std::map<std::string, std::any> launchOptions);
    virtual void applicationWillTerminate(UIApplication* application) {}

    virtual void applicationWillEnterForeground(UIApplication* application) {}
    virtual void applicationDidBecomeActive(UIApplication* application) {}

    virtual void applicationWillResignActive(UIApplication* application) {}
    virtual void applicationDidEnterBackground(UIApplication* application) {}

    virtual void applicationNeedsXIBRegistration(UIApplication* application);
};

}
