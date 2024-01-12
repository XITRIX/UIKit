//
// Created by Даниил Виноградов on 12.01.2024.
//

#include <UIScrollView/RubberBand.hpp>

namespace UIKit {

Point RubberBand::clamp(Point point) const{
    auto x = Geometry::rubberBandClamp(point.x, coeff, dims.width, bounds.minX(), bounds.maxX());
    auto y = Geometry::rubberBandClamp(point.y, coeff, dims.height, bounds.minY(), bounds.maxY());
    return Point(x, y);
}

}