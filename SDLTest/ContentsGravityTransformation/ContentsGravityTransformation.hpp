//
//  ContentsGravityTransformation.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 26.02.2023.
//

#pragma once

#include <Geometry/Geometry.hpp>

namespace UIKit {


enum class CALayerContentsGravity {
    bottom, bottomLeft, bottomRight,
    center, left, right,
    top, topLeft, topRight,
    resize, resizeAspect, resizeAspectFill
};

class CALayer;
struct ContentsGravityTransformation {
    Point offset;
    Size scale;

    ContentsGravityTransformation(CALayer* layer);
    
private:
    Size defaultScale = Size(1.0, 1.0);
};

}
