//
//  UIViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIViewController/UIViewController.hpp>

namespace UIKit {

std::shared_ptr<UIView> UIViewController::view() {
    loadViewIfNeeded();
    return _view;
}

void UIViewController::setView(std::shared_ptr<UIView> view) {
    _view = view;
    view->next = shared_from_this();
    viewDidLoad();
}

bool UIViewController::viewIsLoaded() {
    return _view != nullptr;
}

void UIViewController::loadViewIfNeeded() {
    if (!viewIsLoaded()) {
        loadView();
    }
}

void UIViewController::loadView() {
    setView(std::make_shared<UIView>());
}

}
