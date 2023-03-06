//
//  CASpringAnimation.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include "CASpringAnimation.hpp"

namespace UIKit {

CASpringAnimation::CASpringAnimation(CASpringAnimation* animation):
    CABasicAnimation(animation),
    damping(animation->damping),
    initialSpringVelocity(animation->initialSpringVelocity)
{ }

CASpringAnimation::CASpringAnimation(std::shared_ptr<CASpringAnimationPrototype> prototype,
                                     std::string keyPath,
                                     AnimatableProperty fromValue):
    CABasicAnimation(prototype, keyPath, fromValue, std::make_shared<CAMediaTimingFunction>(CAMediaTimingFunction::easeOutElastic)),
    damping(prototype->damping),
    initialSpringVelocity(prototype->initialSpringVelocity)
{ }

}
