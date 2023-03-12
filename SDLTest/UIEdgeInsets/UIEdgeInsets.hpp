//
//  UIEdgeInsets.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 12.03.2023.
//

#pragma once

#include <Geometry/Geometry.hpp>

namespace UIKit {

struct UIEdgeInsets {
    float top;
    float left;
    float bottom;
    float right;

    UIEdgeInsets(float top, float left, float bottom, float right);
    UIEdgeInsets(): UIEdgeInsets(0, 0, 0, 0) {}

    bool operator==(const UIEdgeInsets& rhs) const;
};

}
