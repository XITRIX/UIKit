//
//  UIWindow.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIWindow/UIWindow.hpp>
#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>

namespace UIKit {

UIWindow::UIWindow() {
    setBackgroundColor(UIColor::systemBackground);
}

void UIWindow::setRootViewController(std::shared_ptr<UIViewController> rootViewController) {
    _rootViewController = rootViewController;
    if (rootViewController)
        rootViewController->next = shared_from_this();
}

void UIWindow::makeKeyAndVisible() {
//    self.safeAreaInsets = UIWindow.getSafeAreaInsets()
    auto window = std::static_pointer_cast<UIWindow>(shared_from_this());
    window->setBounds(UIRenderer::main()->bounds());
    UIApplication::shared->keyWindow = window;

    auto viewController = _rootViewController;
    if (viewController) {
        viewController->loadViewIfNeeded();
        viewController->next = shared_from_this(); // set responder before viewWillAppear etc.
        viewController->view()->setFrame(this->bounds());
        viewController->viewWillAppear(false);
        addSubview(viewController->view());
        viewController->viewDidAppear(false);
    }
}

}
