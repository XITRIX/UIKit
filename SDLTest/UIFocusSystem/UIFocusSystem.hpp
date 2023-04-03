//
//  UIFocusSystem.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 02.04.2023.
//

#pragma once

#include <UIFocus/UIFocus.hpp>
#include <UIEvent/UIEvent.hpp>

namespace UIKit {

class UIView;
class UIFocusSystem {
public:
    UIFocusSystem();
    std::weak_ptr<UIFocusItem> focusedItem() { return _focusedItem; }
private:
    std::weak_ptr<UIFocusItem> _focusedItem;
    std::weak_ptr<UIWindow> _rootWindow;

    void updateFocus();
    void sendEvent(std::shared_ptr<UIEvent> event);

    std::shared_ptr<UIFocusItem> findItemToFocus();
    void applyFocusToItem(std::shared_ptr<UIFocusItem> item, UIFocusUpdateContext context);

    UIFocusHeading makeFocusHeadingFromEvent(std::shared_ptr<UIEvent> event);
    std::shared_ptr<UIFocusItem> searchForFocusInView(std::shared_ptr<UIView> view);

    friend class UIWindow;
};

}
