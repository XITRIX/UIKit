//
//  UIWindow.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIViewController/UIViewController.hpp>
#include <UIFocusSystem/UIFocusSystem.hpp>

namespace UIKit {

class UIWindow: public UIView {
public:
    UIWindow();
    ~UIWindow();

    std::shared_ptr<UIWindow> window() override;
    std::shared_ptr<UIFocusSystem> focusSystem() { return _focusSystem; }

    void setRootViewController(std::shared_ptr<UIViewController> controller);
    std::shared_ptr<UIViewController> rootViewController() { return _rootViewController; }

    void makeKeyAndVisible();
    void sendEvent(std::shared_ptr<UIEvent> event);

    void layoutSubviews() override;
    void updateFocus();

    virtual void pressesBegan(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) override;
    virtual void pressesChanged(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) override;
    virtual void pressesEnded(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) override;
    virtual void pressesCancelled(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) override;
private:
    std::shared_ptr<UIViewController> _rootViewController;
    std::vector<std::shared_ptr<UIViewController>> _presentedViewControllers;

    friend class UIViewController;
    void addPresentedViewController(std::shared_ptr<UIViewController> controller);
    void removePresentedViewController(std::shared_ptr<UIViewController> controller);

    void sendTouchEvent(std::shared_ptr<UIEvent> event);
    void sendPressEvent(std::shared_ptr<UIPressesEvent> event);

    std::shared_ptr<UIFocusSystem> _focusSystem;
};

}
