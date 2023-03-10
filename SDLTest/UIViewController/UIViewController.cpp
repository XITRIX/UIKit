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
    setView(new_shared<UIView>());
}

void UIViewController::addChild(std::shared_ptr<UIViewController> child) {
    _children.push_back(child);
    child->willMoveToParent(weak_from_this().lock());
    child->viewWillAppear(true);
}

void UIViewController::willMoveToParent(std::shared_ptr<UIViewController> parent) {
    if (parent)
        this->_parent = parent;
}

void UIViewController::didMoveToParent(std::shared_ptr<UIViewController> parent) {
    viewDidAppear(true);
}

void UIViewController::removeFromParent() {
    if (auto spt = _parent.lock()) {
        spt->_children.erase(std::remove(spt->_children.begin(), spt->_children.end(), shared_from_this()));
        this->_parent.reset();
        viewDidDisappear(true);
    }
}

}
