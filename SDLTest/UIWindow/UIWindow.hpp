//
//  UIWindow.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIViewController/UIViewController.hpp>

namespace UIKit {

class UIWindow: public UIView {
public:
    UIWindow();

    void setRootViewController(std::shared_ptr<UIViewController> controller);
    std::shared_ptr<UIViewController> rootViewController() { return _rootViewController; }

    void makeKeyAndVisible();
private:
    std::shared_ptr<UIViewController> _rootViewController;
};

}
