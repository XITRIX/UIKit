//
//  ContentsGravityTransformation.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 26.02.2023.
//

#include <ContentsGravityTransformation/ContentsGravityTransformation.hpp>
#include <CALayer/CALayer.hpp>

namespace UIKit {

ContentsGravityTransformation::ContentsGravityTransformation(CALayer* layer) {
    auto scaledContents = Size(
        layer->contents->size().width / layer->contentsScale,
        layer->contents->size().height / layer->contentsScale
    );

    auto bounds = layer->bounds();
    
    auto distanceToMinX = -((bounds.width() - scaledContents.width) * layer->anchorPoint().x);

    auto distanceToMinY = -((bounds.height() - scaledContents.height) * layer->anchorPoint().y);

    auto distanceToMaxX = (bounds.width() - scaledContents.width) * (1 - layer->anchorPoint().x);

    auto distanceToMaxY = (bounds.height() - scaledContents.height) * (1 - layer->anchorPoint().y);

    switch (layer->contentsGravity) {
        case CALayerContentsGravity::resize: {
            this->offset = Point::zero;
            scale = Size(bounds.width() / scaledContents.width, bounds.height() / scaledContents.height);
            break;
        }
        case CALayerContentsGravity::resizeAspectFill: {
            offset = Point::zero;
            auto maxScale = std::max(bounds.width() / scaledContents.width, bounds.height() / scaledContents.height);
            scale = Size(maxScale, maxScale);
            break;
        }
        case CALayerContentsGravity::resizeAspect: {
            offset = Point::zero;
            auto minScale = std::min(bounds.width() / scaledContents.width, bounds.height() / scaledContents.height);
            scale = Size(minScale, minScale);
            break;
        }
        case CALayerContentsGravity::center: {
            offset = Point::zero;
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::left: {
            offset = Point(distanceToMinX, 0.0);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::right: {
            offset = Point(distanceToMaxX, 0.0);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::top: {
            offset = Point(0.0, distanceToMinY);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::bottom: {
            offset = Point(0.0, distanceToMaxY);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::topLeft: {
            offset = Point(distanceToMinX, distanceToMinY);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::topRight: {
            offset = Point(distanceToMaxX, distanceToMinY);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::bottomLeft: {
            offset = Point(distanceToMinX, distanceToMaxY);
            scale = defaultScale;
            break;
        }
        case CALayerContentsGravity::bottomRight: {
            offset = Point(distanceToMaxX, distanceToMaxY);
            scale = defaultScale;
            break;
        }
    }
}

}
