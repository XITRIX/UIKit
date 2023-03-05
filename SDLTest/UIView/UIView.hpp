//
//  UIView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 05.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>
#include <CABasicAnimationPrototype/CABasicAnimationPrototype.hpp>
#include <memory>
#include <set>

namespace UIKit {

class UIView {
public:
    // Animations
    static std::set<std::shared_ptr<CALayer>> layersWithAnimations;
    static std::shared_ptr<CABasicAnimationPrototype> currentAnimationPrototype;
};

}
