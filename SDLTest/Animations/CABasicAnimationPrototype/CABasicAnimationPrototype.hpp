//
//  CABasicAnimationPrototype.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

#include <UIViewAnimationGroup/UIViewAnimationGroup.hpp>
#include <AnimatableProperty/AnimatableProperty.hpp>
#include <memory>

namespace UIKit {

class CABasicAnimation;
class CABasicAnimationPrototype: public std::enable_shared_from_this<CABasicAnimationPrototype> {
public:
    const double duration;
    const double delay;
    const std::shared_ptr<UIViewAnimationGroup> animationGroup;

    CABasicAnimationPrototype(double duration, double delay, std::shared_ptr<UIViewAnimationGroup> animationGroup);

    std::shared_ptr<CABasicAnimation> createAnimation(std::string keyPath, AnimatableProperty fromValue);
};

}