//
//  UIEvent.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#include <UIEvent/UIEvent.hpp>
#include <UITouch/UITouch.hpp>

namespace UIKit {

std::vector<std::shared_ptr<UIEvent>> UIEvent::activeEvents;

UIEvent::UIEvent() {};
UIEvent::UIEvent(std::shared_ptr<UITouch> touch) {
    _allTouches.insert(touch);
}

}
