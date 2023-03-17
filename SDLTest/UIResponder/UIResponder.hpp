//
//  UIResponder.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <memory>
#include <set>

namespace UIKit {

class UITouch;
class UIEvent;
class UIResponder {
public:
    virtual ~UIResponder() {}
    virtual std::shared_ptr<UIResponder> next() = 0;

    virtual void touchesBegan(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesMoved(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesEnded(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
    virtual void touchesCancelled(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event);
};

}
