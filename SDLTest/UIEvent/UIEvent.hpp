//
//  UIEvent.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#pragma once

#include <Tools/Tools.hpp>
#include <set>

namespace UIKit {

class UITouch;
class UIEvent {
public:
    std::set<std::shared_ptr<UITouch>> allTouches() { return _allTouches; }
    Time timestamp() { return _timestamp; }

    UIEvent();
    virtual ~UIEvent() {}
private:
    UIEvent(std::shared_ptr<UITouch> touch);

    static std::vector<std::shared_ptr<UIEvent>> activeEvents;
    std::set<std::shared_ptr<UITouch>> _allTouches;
    Time _timestamp = getCPUTimeUsec();

    friend class UIApplication;
    friend class UIWindow;
};

}
