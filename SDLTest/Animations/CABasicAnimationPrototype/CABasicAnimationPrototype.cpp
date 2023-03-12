//
//  CABasicAnimationPrototype.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <CABasicAnimation/CABasicAnimation.hpp>
#include <Tools/Tools.hpp>

namespace UIKit {

CABasicAnimationPrototype::CABasicAnimationPrototype(double duration, double delay, std::shared_ptr<UIViewAnimationGroup> animationGroup):
    duration(duration), delay(delay), animationGroup(animationGroup)
{ }

std::shared_ptr<CABasicAnimation> CABasicAnimationPrototype::createAnimation(std::string keyPath, AnimatableProperty fromValue) {
    return new_shared<CABasicAnimation>(
        shared_from_this(),
        keyPath,
        fromValue,
        CAMediaTimingFunction::timingFunctionFrom(animationGroup->options)
    );
}

}
