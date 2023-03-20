//
//  UIViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIViewController/UIViewController.hpp>
#include <DispatchQueue/DispatchQueue.hpp>

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
    _view->_parentController = weak_from_this();
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

void UIViewController::viewWillAppear(bool animated) {
    for (auto& child: _children) {
        DispatchQueue::main()->async([child, animated]() { child->viewWillAppear(animated); });
    }
}

void UIViewController::viewDidAppear(bool animated) {
    for (auto& child: _children) {
        DispatchQueue::main()->async([child, animated]() { child->viewDidAppear(animated); });
    }
}

void UIViewController::viewWillDisappear(bool animated) {
    for (auto& child: _children) {
        DispatchQueue::main()->async([child, animated]() { child->viewWillDisappear(animated); });
    }
}

void UIViewController::viewDidDisappear(bool animated) {
    for (auto& child: _children) {
        DispatchQueue::main()->async([child, animated]() { child->viewDidDisappear(animated); });
    }
}

void UIViewController::addChild(std::shared_ptr<UIViewController> child) {
    _children.push_back(child);
    child->willMoveToParent(weak_from_this().lock());
}

void UIViewController::willMoveToParent(std::shared_ptr<UIViewController> parent) {
    if (parent)
        this->_parent = parent;
}

void UIViewController::didMoveToParent(std::shared_ptr<UIViewController> parent) {
    if (parent->view()->window())
        viewDidAppear(true);
}

void UIViewController::removeFromParent() {
    if (auto spt = _parent.lock()) {
        spt->_children.erase(std::remove(spt->_children.begin(), spt->_children.end(), shared_from_this()));
        this->_parent.reset();
        viewDidDisappear(true);
    }
}

void UIViewController::setAdditionalSafeAreaInsets(UIEdgeInsets additionalSafeAreaInsets) {
    if (_additionalSafeAreaInsets == additionalSafeAreaInsets) return;
    _additionalSafeAreaInsets = additionalSafeAreaInsets;
    view()->setNeedsUpdateSafeAreaInsets();
}

void UIViewController::setViewRespectsSystemMinimumLayoutMargins(bool viewRespectsSystemMinimumLayoutMargins) {
    if (_viewRespectsSystemMinimumLayoutMargins == viewRespectsSystemMinimumLayoutMargins) return;
    _viewRespectsSystemMinimumLayoutMargins = viewRespectsSystemMinimumLayoutMargins;
    view()->setNeedsUpdateLayoutMargins();
}

}
