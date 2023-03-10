//
//  UIViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIView/UIView.hpp>

namespace UIKit {

class UIViewController: public UIResponder, public enable_shared_from_this<UIViewController> {
public:
    void setView(std::shared_ptr<UIView> view);
    std::shared_ptr<UIView> view();
    bool viewIsLoaded();
    void loadViewIfNeeded();

    std::shared_ptr<UIResponder> next() override;

    // Most of these methods are designed to be overriden in `UIViewController` subclasses
    virtual void loadView();
    virtual void viewDidLoad() {}
    virtual void viewWillAppear(bool animated) {}
    virtual void viewDidAppear(bool animated) {}
    virtual void viewWillDisappear(bool animated) {}
    virtual void viewDidDisappear(bool animated) {}

    virtual void viewWillLayoutSubviews() {}
    virtual void viewDidLayoutSubviews() {}

    std::vector<std::shared_ptr<UIViewController>> children() { return _children; }
    std::weak_ptr<UIViewController> parent() { return _parent; }

    void addChild(std::shared_ptr<UIViewController> child);
    virtual void willMoveToParent(std::shared_ptr<UIViewController> parent);
    virtual void didMoveToParent(std::shared_ptr<UIViewController> parent);
    void removeFromParent();

private:
    std::shared_ptr<UIView> _view;
    std::weak_ptr<UIViewController> _parent;
    std::vector<std::shared_ptr<UIViewController>> _children;
};

}
