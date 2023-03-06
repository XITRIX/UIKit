//
//  UIViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIView/UIView.hpp>

namespace UIKit {

class UIViewController: public UIResponder, public std::enable_shared_from_this<UIViewController> {
public:
    void setView(std::shared_ptr<UIView> view);
    std::shared_ptr<UIView> view();
    bool viewIsLoaded();
    void loadViewIfNeeded();

    // Most of these methods are designed to be overriden in `UIViewController` subclasses
    virtual void loadView();
    virtual void viewDidLoad() {}
    virtual void viewWillAppear(bool animated) {}
    virtual void viewDidAppear(bool animated) {}
    virtual void viewWillDisappear(bool animated) {}
    virtual void viewDidDisappear(bool animated) {}

    virtual void viewWillLayoutSubviews() {}
    virtual void viewDidLayoutSubviews() {}

private:
    std::shared_ptr<UIView> _view;
};

}
