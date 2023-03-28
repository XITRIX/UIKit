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
    ~UIWindow();

    std::shared_ptr<UIWindow> window() override;

    void setRootViewController(std::shared_ptr<UIViewController> controller);
    std::shared_ptr<UIViewController> rootViewController() { return _rootViewController; }

    void makeKeyAndVisible();
    void sendEvent(std::shared_ptr<UIEvent> event);

    void layoutSubviews() override;
private:
    std::shared_ptr<UIViewController> _rootViewController;
    std::vector<std::shared_ptr<UIViewController>> _presentedViewControllers;

    friend class UIViewController;
    void addPresentedViewController(std::shared_ptr<UIViewController> controller);
    void removePresentedViewController(std::shared_ptr<UIViewController> controller);
};

}
