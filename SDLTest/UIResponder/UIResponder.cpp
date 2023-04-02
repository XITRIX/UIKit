//
//  UIResponder.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIResponder/UIResponder.hpp>
#include <UITouch/UITouch.hpp>

namespace UIKit {

void UIResponder::touchesBegan(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    auto next = this->next();
    if (next) next->touchesBegan(touches, event);
}

void UIResponder::touchesMoved(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    auto next = this->next();
    if (next) next->touchesMoved(touches, event);
}

void UIResponder::touchesEnded(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    auto next = this->next();
    if (next) next->touchesEnded(touches, event);
}

void UIResponder::touchesCancelled(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
    auto next = this->next();
    if (next) next->touchesCancelled(touches, event);
}

void UIResponder::pressesBegan(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) {
    auto next = this->next();
    if (next) next->pressesBegan(pressees, event);
}

void UIResponder::pressesChanged(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) {
    auto next = this->next();
    if (next) next->pressesChanged(pressees, event);
}

void UIResponder::pressesEnded(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) {
    auto next = this->next();
    if (next) next->pressesEnded(pressees, event);
}

void UIResponder::pressesCancelled(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) {
    auto next = this->next();
    if (next) next->pressesCancelled(pressees, event);
}

}
