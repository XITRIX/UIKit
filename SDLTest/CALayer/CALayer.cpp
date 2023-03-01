//
//  CALayer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <CALayer/CALayer.hpp>
#include <UIApplication/UIApplication.hpp>
#include <ShaderProgram/ShaderProgram.hpp>
#include <algorithm>

namespace UIKit {

CALayer::CALayer() { }

CALayer::~CALayer() {
    if (groupingFBO)
        GPU_FreeImage(groupingFBO);
}

void CALayer::draw(GPU_Target *renderer) { }

void CALayer::render(GPU_Target* renderer) {
    refreshGroupingFBO();

    if (opacity < 0.001f) { return; }


    auto localRenderer = renderer;
    if (groupingFBO) {
        localRenderer = groupingFBO->target;
        GPU_SetActiveTarget(localRenderer);
    } else {
        // TODO: Add alpha if groupingFBO is disabled
    }


    // The basis for all our transformations is `position` (in parent coordinates), which in this layer's
    // coordinates is `anchorPoint`. To make this happen, we translate (in our parent's coordinate system
    // – which may in turn be affected by its parents, and so on) to `position`, and then render rectangles
    // which may (and often do) start at a negative `origin` based on our (bounds) `size` and `anchorPoint`:
    auto parentOriginTransform = NXTransform3D(GPU_GetCurrentMatrix());
    auto translationToPosition = CATransform3DMakeTranslation(position.x, position.y, zPosition);
    auto transformAtPositionInParentCoordinates = parentOriginTransform * translationToPosition;

    auto modelViewTransform = transformAtPositionInParentCoordinates * transform;

    // Now that we're in our own coordinate system based around `anchorPoint` (which is generally the middle of
    // bounds.size), we need to find the top left of the rectangle in order to be able to render rectangles.
    // Since we have already applied our own `transform`, we can work in our own (`bounds.size`) units.
    auto deltaFromAnchorPointToOrigin = Point(-(bounds.width() * anchorPoint.x),
                                              -(bounds.height() * anchorPoint.y));
    auto renderedBoundsRelativeToAnchorPoint = Rect(deltaFromAnchorPointToOrigin, bounds.size);

    // Big performance optimization. Don't render anything that's entirely offscreen:
    auto rendererBounds = Rect(0, 0, renderer->w, renderer->h);
    auto absoluteFrame = renderedBoundsRelativeToAnchorPoint.applying(modelViewTransform);
    if (!absoluteFrame.intersects(rendererBounds)) { return; }

    modelViewTransform.setAsSDLgpuMatrix();

    // `position` is always relative from the parent's origin, but the global GPU matrix is currently
    // focused on `self.position` rather than the `origin` we calculated to render rectangles.
    // We need to be at `origin` here though so we can translate to the next `position` in each sublayer.
    //
    // We also subtract `bounds` to get the strange but useful scrolling effect as on iOS.
    auto translationFromAnchorPointToOrigin = CATransform3DMakeTranslation(
        deltaFromAnchorPointToOrigin.x - bounds.origin.x,
        deltaFromAnchorPointToOrigin.y - bounds.origin.y,
        0 // If we moved (e.g.) forward to render `self`, all sublayers should start at that same zIndex
    );

    // This transform is referred to as the `parentOriginTransform` in our sublayers (see above):
    auto transformAtSelfOrigin = modelViewTransform * translationFromAnchorPointToOrigin;

    if (mask) {
//        auto maskFrame = (mask._presentation ?? mask).frame;
//        auto maskFrame = mask->getFrame();
//        auto maskAbsoluteFrame = maskFrame.offsetBy(absoluteFrame.origin);

        // Don't intersect with previousClippingRect: in a case where both `masksToBounds` and `mask` are
        // present, using previousClippingRect would not constrain the area as much as it might otherwise
//        renderer.clippingRect =
//            renderer.clippingRect?.intersection(maskAbsoluteFrame) ?? maskAbsoluteFrame

        GPU_PushMatrix();
        transformAtSelfOrigin.setAsSDLgpuMatrix();

        // Create new FBO for mask texture
        if (!maskFBO || maskFBO->size() != bounds.size) {
            maskFBO = std::make_shared<CGImage>(bounds.size);
        }

        // Setup FBO for mask texture
        GPU_GetTarget(maskFBO->pointee);
        GPU_SetActiveTarget(maskFBO->pointee->target);
        GPU_Clear(maskFBO->pointee->target);

        // Render mask texture
        mask->render(maskFBO->pointee->target);

        // Reset to old render target
        GPU_SetActiveTarget(localRenderer);
        GPU_PopMatrix();

        // Apply mask
        ShaderProgram::getMask()->activate(); // must activate before setting parameters (below)!
        ShaderProgram::getMask()->setMaskImage(maskFBO, mask->getRenderedBoundsRelativeToAnchorPoint());
    } else { maskFBO = nullptr; }

    // Background color
    if (cornerRadius <= 0.001f) {
        GPU_RectangleFilled2(localRenderer, renderedBoundsRelativeToAnchorPoint.gpuRect(), backgroundColor.color);
    } else {
        GPU_RectangleRoundFilled2(localRenderer, renderedBoundsRelativeToAnchorPoint.gpuRect(), cornerRadius, backgroundColor.color);
    }

    // Contents
    if (contents) {
        auto contentsGravity = ContentsGravityTransformation(this);
        GPU_SetAnchor(contents->pointee, anchorPoint.x, anchorPoint.y);
        GPU_SetRGBA(contents->pointee, 255, 255, 255, opacity * 255);

        GPU_BlitTransform(
            contents->pointee,
            NULL,
            renderer,
            contentsGravity.offset.x,
            contentsGravity.offset.y,
            0, // rotation in degrees
            contentsGravity.scale.width / contentsScale,
            contentsGravity.scale.height / contentsScale
        );
    }

    if (mask) {
        ShaderProgram::deactivateAll();
    }

    draw(localRenderer);

    // Apply transform for subviews
    transformAtSelfOrigin.setAsSDLgpuMatrix();
    for (auto sublayer: sublayers) {
        sublayer->render(localRenderer);
    }

    parentOriginTransform.setAsSDLgpuMatrix();

    if (groupingFBO) {
        GPU_SetActiveTarget(renderer);
        GPU_SetRGBA(groupingFBO, 255, 255, 255, opacity * 255);
        GPU_Blit(groupingFBO, nullptr, renderer, 0, 0);
    }


}

Rect CALayer::getFrame() {
    // Create a rectangle based on `bounds.size` * `transform` at `position` offset by `anchorPoint`
    auto transformedBounds = bounds.applying(transform);

    auto anchorPointOffset = Point(
        transformedBounds.width() * anchorPoint.x,
        transformedBounds.height() * anchorPoint.y
                                   );

    return Rect(position.x - anchorPointOffset.x,
                position.y - anchorPointOffset.y,
                transformedBounds.width(),
                transformedBounds.height());
}

void CALayer::setFrame(Rect frame) {
    position = Point(frame.origin.x + (frame.width() * anchorPoint.x),
                     frame.origin.y + (frame.height() * anchorPoint.y));

    auto inverseTransformOpt = affineTransform().inverted();
    if (!inverseTransformOpt.has_value()) {
//        assertionFailure("You tried to set the frame of a CALayer whose transform cannot be inverted. This is undefined behaviour.");
        return;
    }
    auto inverseTransform = inverseTransformOpt.value();


    // If we are shrinking the view with a transform and then setting a
    // new frame, the layer's actual `bounds` is bigger (and vice-versa):
    auto nonTransformedBoundSize = frame.applying(inverseTransform).size;
    bounds.size = nonTransformedBoundSize;
}

float CALayer::getOpacity() const {
    return opacity;
}

void CALayer::setOpacity(float opacity) {
    this->opacity = opacity;
}

void CALayer::setMask(ptr<CALayer> mask) {
    this->mask = mask;
    if (mask) mask->superlayer = shared_from_this();
}

ptr<CALayer> CALayer::getMask() const {
    return mask;
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

void CALayer::refreshGroupingFBO() {
    if (!allowsGroupOpacity || opacity >= 1 || opacity < 0.001f) {
        GPU_FreeImage(groupingFBO);
        groupingFBO = nullptr;
        return;
    }

    auto currentTarget = GPU_GetActiveTarget();
    if (!groupingFBO) {
        groupingFBO = GPU_CreateImage(currentTarget->w, currentTarget->h, GPU_FORMAT_RGBA);
        GPU_SetAnchor(groupingFBO, 0, 0);
        GPU_GetTarget(groupingFBO);
        return;
    }

    if (groupingFBO->w != currentTarget->w || groupingFBO->h != currentTarget->h) {
        GPU_FreeImage(groupingFBO);
        groupingFBO = GPU_CreateImage(currentTarget->w, currentTarget->h, GPU_FORMAT_RGBA);
        GPU_SetAnchor(groupingFBO, 0, 0);
        GPU_GetTarget(groupingFBO);
        return;
    }
}


NXAffineTransform CALayer::affineTransform() {
    return NXTransform3DGetAffineTransform(transform);
}

void CALayer::setAffineTransform(NXAffineTransform transform) {
    this->transform = NXTransform3DMakeAffineTransform(transform);
}

Rect CALayer::getRenderedBoundsRelativeToAnchorPoint() {
    auto deltaFromAnchorPointToOrigin = Point(-(bounds.width() * anchorPoint.x),
                                              -(bounds.height() * anchorPoint.y));
    return Rect(deltaFromAnchorPointToOrigin, bounds.size);
}

}
