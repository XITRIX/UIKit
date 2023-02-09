//
//  CALayer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>

namespace UIKit {

CALayer::CALayer() {
    updateContents();
}

void CALayer::render(SDL_Renderer *renderer, Point globalOffset) {
    auto frame = getFrame().offsetBy(globalOffset);
    SDL_Rect rect;
    rect.x = frame.minX();
    rect.y = frame.minY();
    rect.w = frame.width();
    rect.h = frame.height();

    SDL_RenderCopyEx(renderer, contents->pointee, NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);

    for (auto layer : sublayers) {
        layer->render(renderer, frame.origin);
    }
}

void CALayer::draw(SDL_Renderer *renderer) {
    // Set renderer color red to draw the square
    //    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x3F);
    //
    //    SDL_Rect rect;
    //    rect.x = 10;
    //    rect.y = 10;
    //    rect.w = 30;
    //    rect.h = 30;
    //
    //    // Draw filled square
    //    SDL_RenderFillRect(renderer, &rect);
}

void CALayer::internalDraw(SDL_Renderer* renderer) {
    updateContents();

    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

    SDL_SetRenderTarget(renderer, contents->pointee);
//    SDL_SetTextureAlphaMod(contents->pointee, opacity * 255);
    SDL_SetRenderDrawColor(renderer, backgroundColor.color.r, backgroundColor.color.g, backgroundColor.color.b, backgroundColor.color.a);

    SDL_RenderClear(renderer);

    draw(renderer);

    SDL_SetRenderTarget(renderer, oldTarget);
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

}

void CALayer::insertSublayerBelow(ptr<CALayer> layer, ptr<CALayer> sibling) {

}

void CALayer::removeFromSuperlayer() {
    auto super = superlayer.lock();
    if (super == nullptr) return;

    super->sublayers.erase(std::remove(super->sublayers.begin(), super->sublayers.end(), shared_from_this()), super->sublayers.end());
}

void CALayer::updateContents() {
    if (contents == nullptr || bounds.size != contents->size()) {
        if (bounds.size == Size()) return;
        contents = std::make_shared<CGImage>(bounds.size);
    }
}

}
