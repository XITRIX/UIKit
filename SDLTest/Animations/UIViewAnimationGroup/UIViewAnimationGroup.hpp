//
//  UIViewAnimationGroup.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

#include <UIViewAnimationOptions/UIViewAnimationOptions.hpp>
#include <optional>
#include <functional>

namespace UIKit {

class UIViewAnimationGroup {
public:
    std::optional<std::function<void(bool)>> completion;
    int queuedAnimations = 0;
    UIViewAnimationOptions options;

    UIViewAnimationGroup(UIViewAnimationOptions options, std::optional<std::function<void(bool)>> completion);

    void animationDidStop(bool finished);
};

}
