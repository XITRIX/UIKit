//
//  CALayer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>
#include <algorithm>

namespace UIKit {

CALayer::CALayer() {
}

void CALayer::draw(GPU_Target *renderer) {}

void CALayer::render(GPU_Target* renderer) {
    auto image = GPU_CreateImage(renderer->w, renderer->h, GPU_FORMAT_RGBA);
    auto localRenderer = GPU_LoadTarget(image);
    GPU_SetActiveTarget(localRenderer);

    auto parentOriginTransform = NXTransform3D(GPU_GetCurrentMatrix());
    auto translationToPosition = CATransform3DMakeTranslation(position.x, position.y, zPosition);
    auto transformAtPositionInParentCoordinates = parentOriginTransform * translationToPosition;

    auto modelViewTransform = transformAtPositionInParentCoordinates * transform;

    modelViewTransform.setAsSDLgpuMatrix();

    auto frame = getFrame();
    GPU_RectangleFilled2(renderer, GPU_MakeRect(frame.minX(), frame.minY(), frame.width(), frame.height()), backgroundColor.color);

    draw(renderer);

    for (auto sublayer: sublayers) {
        sublayer->render(localRenderer);
    }

    parentOriginTransform.setAsSDLgpuMatrix();

    GPU_SetActiveTarget(renderer);

    GPU_Blit(image, nullptr, renderer, 0, 0);

    GPU_FreeTarget(localRenderer);
    GPU_FreeImage(image);
}

Rect CALayer::getFrame() {
    return Rect(position, bounds.size);
}

void CALayer::setFrame(Rect frame) {
    position = frame.origin;
    bounds.size = frame.size;
}

float CALayer::getOpacity() const {
    return opacity;
}

void CALayer::setOpacity(float opacity) {
    this->opacity = opacity;
}

void CALayer::addSublayer(ptr<CALayer> layer) {
    layer->removeFromSuperlayer();
    sublayers.push_back(layer);
    layer->superlayer = this->shared_from_this();
}

void CALayer::insertSublayerAt(ptr<CALayer> layer, int index) {
    layer->removeFromSuperlayer();
    sublayers.insert(sublayers.begin() + index, layer);
    layer->superlayer = this->shared_from_this();
}

void CALayer::insertSublayerAbove(ptr<CALayer> layer, ptr<CALayer> sibling) {
    // TODO: Need to implement
}

void CALayer::insertSublayerBelow(ptr<CALayer> layer, ptr<CALayer> sibling) {
    // TODO: Need to implement
}

void CALayer::removeFromSuperlayer() {
    auto super = superlayer.lock();
    if (super == nullptr) return;

    super->sublayers.erase(std::remove(super->sublayers.begin(), super->sublayers.end(), shared_from_this()), super->sublayers.end());
}

}
