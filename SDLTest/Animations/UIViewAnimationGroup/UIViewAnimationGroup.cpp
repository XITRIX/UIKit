//
//  UIViewAnimationGroup.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#include <UIViewAnimationGroup/UIViewAnimationGroup.hpp>

namespace UIKit {

UIViewAnimationGroup::UIViewAnimationGroup(UIViewAnimationOptions options, std::optional<std::function<void(bool)>> completion):
    options(options), completion(completion)
{ }

void UIViewAnimationGroup::animationDidStop(bool finished) {
    queuedAnimations -= 1;
    if (queuedAnimations == 0) {
        if (completion.has_value())
            completion.value()(finished);
    }
}

}
