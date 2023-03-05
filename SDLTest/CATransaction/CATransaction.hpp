//
//  CATransaction.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

#include <CALayer/CALayer.hpp>

namespace UIKit {

struct CATransaction {
private:
    bool disableActions_ = false;
    float animationDuration_ = CALayer::defaultAnimationDuration;

    static std::vector<CATransaction> transactionStack;

public:
    static void begin();

    static void commit();

    static bool disableActions();

    static void setDisableActions(bool newValue);

    static float animationDuration();

    static void setAnimationDuration(float newValue);
};

}
