//
//  CABasicAnimation.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

#include <CAAction/CAAction.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <CAMediaTimingFunction/CAMediaTimingFunction.hpp>
#include <AnimatableProperty/AnimatableProperty.hpp>
#include <Timer/Timer.hpp>
#include <string>
#include <optional>

namespace UIKit {

class CABasicAnimation: public CAAction {
public:
    /// animation duration in seconds
    double duration = 0;

    /// animation delay in seconds
    double delay = 0;

    std::optional<std::string> keyPath;
    std::optional<std::string> fillMode;
    bool isRemovedOnCompletion = true;
    std::shared_ptr<CAMediaTimingFunction> timingFunction;

    std::optional<AnimatableProperty> fromValue;
    std::optional<AnimatableProperty> toValue;

    std::shared_ptr<UIViewAnimationGroup> animationGroup;
    Timer creationTime;

    CABasicAnimation(std::string keyPath);
    CABasicAnimation(std::shared_ptr<CABasicAnimationPrototype> prototype, std::string keyPath, AnimatableProperty fromValue, std::shared_ptr<CAMediaTimingFunction> timingFunction);
    CABasicAnimation(CABasicAnimation* animation);

    bool wasCreatedInUIAnimateBlock();
    float progressFor(Timer currentTime);

private:
    float ease(float x);
};

}
