//
//  UIControl.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 03.04.2023.
//

#pragma once

#include <UIView/UIView.hpp>

namespace UIKit {

class UIControl: public UIView {
public:
    UIControl();

    virtual bool canBecomeFocused() override;
    virtual void didUpdateFocusIn(UIFocusUpdateContext context, UIFocusAnimationCoordinator* coordinator) override;

    static std::shared_ptr<UIView> init() { return new_shared<UIControl>(); }
};

}
