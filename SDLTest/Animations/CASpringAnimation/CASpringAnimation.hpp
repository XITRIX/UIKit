//
//  CASpringAnimation.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <CABasicAnimation/CABasicAnimation.hpp>
#include <CASpringAnimationPrototype/CASpringAnimationPrototype.hpp>

namespace UIKit {

class CASpringAnimation: public CABasicAnimation {
public:
    std::optional<double> damping;
    std::optional<double> initialSpringVelocity;

    CASpringAnimation(CASpringAnimation* animation);
    CASpringAnimation(std::shared_ptr<CASpringAnimationPrototype> prototype,
                      std::string keyPath,
                      AnimatableProperty fromValue);
};

}
