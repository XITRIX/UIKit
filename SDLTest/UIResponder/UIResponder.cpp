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

}
