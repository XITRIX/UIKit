//
//  UIFocusAnimationCoordinator.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 02.04.2023.
//

#pragma once

#include <functional>

namespace UIKit {

class UIFocusAnimationContext {
public:
    virtual float duration() { return 0.3f; }
};

class UIFocusAnimationCoordinator {

    /**
     Specifies focus-related animations that should be coordinated with the animations of the focusing or un-focusing view.

     Any animations specified will be run in the same animation context as the main animation. The completion block is invoked after the main animation completes.
     (Note that this may not be after all the coordinated animations specified by the call complete if the duration is not inherited.)

     It is perfectly legitimate to only specify a completion block.
     */
    virtual void addCoordinatedAnimations(std::function<void()> animations, std::function<void()> completion = [](){});

    /**
     Specifies focus-related animations that should be coordinated with the animations of the focusing item.

     Any animations specified will be run in the same animation context as the main animation. The completion block is invoked after the UIKit-defined animations complete.

     It is perfectly legitimate to only specify a completion block.

     A context object is provided in the animation block with details of the UIKit-defined animations being run for the focusing item.
     */
    virtual void addCoordinatedFocusingAnimations(std::function<void(UIFocusAnimationContext)> animations, std::function<void()> completion = [](){});

    /**
     Specifies focus-related animations that should be coordinated with the animations of the un-focusing item.

     Any animations specified will be run in the same animation context as the main animation. The completion block is invoked after the UIKit-defined animations complete.

     It is perfectly legitimate to only specify a completion block.

     A context object is provided in the animation block with details of the UIKit-defined animations being run for the un-focusing item.
     */
    virtual void addCoordinatedUnfocusingAnimations(std::function<void(UIFocusAnimationContext)> animations, std::function<void()> completion = [](){});
};

}
