//
//  UIViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIViewController/UIViewController.hpp>

namespace UIKit {

std::shared_ptr<UIResponder> UIViewController::next() {
    if (!_view->_superview.expired()) {
        return _view->_superview.lock();
    }
    return nullptr;
}

std::shared_ptr<UIView> UIViewController::view() {
    loadViewIfNeeded();
    return _view;
}

void UIViewController::setView(std::shared_ptr<UIView> view) {
    if (_view) _view->_parentController.reset();
    _view = view;
    _view->_parentController = shared_from_this();
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
