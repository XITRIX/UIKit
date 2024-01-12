//
// Created by Даниил Виноградов on 12.01.2024.
//

#pragma once

#include <Geometry/Geometry.hpp>

namespace UIKit {

struct RubberBand {
    float coeff;
    Size dims;
    Rect bounds;

    RubberBand(float coeff, Size dims, Rect bounds):
        coeff(coeff), dims(dims), bounds(bounds) {}

    Point clamp(Point point) const;
};

}