//
//  UIPressesEvent.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 31.03.2023.
//

#pragma once

#include <UIEvent/UIEvent.hpp>

namespace UIKit {

class UIPress;
class UIGestureRecognizer;
class UIPressesEvent: public UIEvent {
public:
    std::set<std::shared_ptr<UIPress>> pressesFor(std::shared_ptr<UIGestureRecognizer> gesture);
    std::set<std::shared_ptr<UIPress>> allPresses() { return _allPresses; }
private:
    UIPressesEvent(std::shared_ptr<UIPress> press);

    static std::vector<std::shared_ptr<UIPressesEvent>> activePressesEvents;
    std::set<std::shared_ptr<UIPress>> _allPresses;

    friend class UIApplication;
};

}
