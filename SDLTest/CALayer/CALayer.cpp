//
//  CALayer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <algorithm>
#include <typeinfo>

#include <CALayer/CALayer.hpp>
#include <UIView/UIView.hpp>
#include <CATransaction/CATransaction.hpp>
#include <UIApplication/UIApplication.hpp>
#include <ShaderProgram/ShaderProgram.hpp>
#include <Renderer/Renderer.hpp>
#include <Utils/Utils.hpp>

namespace UIKit {

float CALayer::defaultAnimationDuration = 0.3f;

CALayer::CALayer() { }

CALayer::CALayer(CALayer* layer) {
    bounds = layer->bounds;
    delegate = layer->delegate;
    transform = layer->transform;
    position = layer->position;
    anchorPoint = layer->anchorPoint;
    opacity = layer->opacity;
    backgroundColor = layer->backgroundColor;
    isHidden = layer->isHidden;
    cornerRadius = layer->cornerRadius;
//    borderWidth = layer->borderWidth;
//    borderColor = layer->borderColor;
//    shadowColor = layer->shadowColor;
//    shadowPath = layer->shadowPath;
//    shadowOffset = layer->shadowOffset;
//    shadowRadius = layer->shadowRadius;
//    shadowOpacity = layer->shadowOpacity;
    mask = layer->mask;
//    masksToBounds = layer->masksToBounds;
    contents = layer->contents; // XXX: we should make a copy here
    contentsScale = layer->contentsScale;
    superlayer = layer->superlayer;
    sublayers = layer->sublayers;
    contentsGravity = layer->contentsGravity;
}

CALayer::~CALayer() {
    if (groupingFBO)
        GPU_FreeImage(groupingFBO);
}

void CALayer::draw(NVGcontext* context) { }

void CALayer::render(GPU_Target* renderer) {
    refreshGroupingFBO();

    if (isHidden || opacity < 0.001f) { return; }

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
        auto maskSize = Size(localRenderer->base_w, localRenderer->base_h);
        if (!maskFBO || maskFBO->size() != maskSize) {
            maskFBO = std::make_shared<CGImage>(maskSize);
            GPU_SetAnchor(maskFBO->pointee, 0, 0);
            GPU_GetTarget(maskFBO->pointee);
            GPU_SetVirtualResolution(maskFBO->pointee->target, localRenderer->w, localRenderer->h);
        }

        // Setup FBO for mask texture
        GPU_SetActiveTarget(maskFBO->pointee->target);
        GPU_Clear(maskFBO->pointee->target);

        // Render mask texture
        mask->render(maskFBO->pointee->target);

        // Reset to old render target
        GPU_SetActiveTarget(localRenderer);
        GPU_PopMatrix();

        // Debug: - draw mask as image
//        auto offset = mask->getRenderedBoundsRelativeToAnchorPoint();
//        auto rect = Rect(offset.minX(), offset.minY(), localRenderer->w, localRenderer->h).gpuRect();
//        GPU_BlitRect(maskFBO->pointee, NULL, localRenderer, &rect);
        // Debug: - draw mask as image

        // Apply mask
        auto offset = mask->getRenderedBoundsRelativeToAnchorPoint();
        auto rect = Rect(offset.minX(), offset.minY(), localRenderer->w, localRenderer->h);
        ShaderProgram::getMask()->activate(); // must activate before setting parameters (below)!
        ShaderProgram::getMask()->setMaskImage(maskFBO, rect);
    } else { maskFBO = nullptr; }

    // Background color
    if (backgroundColor.has_value()) {
        if (cornerRadius <= 0.001f) {
            GPU_RectangleFilled2(localRenderer, renderedBoundsRelativeToAnchorPoint.gpuRect(), backgroundColor.value().color);
        } else {
            GPU_PushMatrix();
            Renderer::shared()->draw([this, renderedBoundsRelativeToAnchorPoint](auto context) {
                auto color = backgroundColor.value().color;
                nvgBeginPath(context);
                nvgFillColor(context, nvgRGBA(color.r, color.g, color.b, color.a));
                nvgRoundedRect(context, renderedBoundsRelativeToAnchorPoint.minX(), renderedBoundsRelativeToAnchorPoint.minY(), renderedBoundsRelativeToAnchorPoint.width(), renderedBoundsRelativeToAnchorPoint.height(), cornerRadius);
                nvgFill(context);
            });
            GPU_PopMatrix();
        }
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
    
    Renderer::shared()->draw([this](auto context) {
        draw(context);
    });

    // Apply transform for subviews
    transformAtSelfOrigin.setAsSDLgpuMatrix();
    for (auto sublayer: sublayers) {
        sublayer->render(localRenderer);
    }

    parentOriginTransform.setAsSDLgpuMatrix();

    if (groupingFBO) {
        GPU_SetActiveTarget(renderer);
        GPU_SetRGBA(groupingFBO, 255, 255, 255, opacity * 255);
//        GPU_Blit(groupingFBO, nullptr, renderer, 0, 0);
        auto rect = GPU_MakeRect(0, 0, renderer->w, renderer->h);
        GPU_BlitRect(groupingFBO, NULL, renderer, &rect);
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

void CALayer::setMask(std::shared_ptr<CALayer> mask) {
    if (this->mask) {
        this->mask->superlayer.reset();
    }
    this->mask = mask;
    if (mask) mask->superlayer = shared_from_this();
}

std::shared_ptr<CALayer> CALayer::getMask() const {
    return mask;
}

void CALayer::addSublayer(std::shared_ptr<CALayer> layer) {
    layer->removeFromSuperlayer();
    sublayers.push_back(layer);
    layer->superlayer = this->shared_from_this();
}

void CALayer::insertSublayerAt(std::shared_ptr<CALayer> layer, int index) {
    layer->removeFromSuperlayer();
    sublayers.insert(sublayers.begin() + index, layer);
    layer->superlayer = this->shared_from_this();
}

void CALayer::insertSublayerAbove(std::shared_ptr<CALayer> layer, std::shared_ptr<CALayer> sibling) {
    // TODO: Need to implement
}

void CALayer::insertSublayerBelow(std::shared_ptr<CALayer> layer, std::shared_ptr<CALayer> sibling) {
    // TODO: Need to implement
}

void CALayer::removeFromSuperlayer() {
    auto super = superlayer.lock();
    if (super == nullptr) return;

    // If it's mask - remove
    if (super->mask.get() == this) {
        super->mask = nullptr;
        return;
    }

    // Find and remove this from superlayer
    super->sublayers.erase(std::remove(super->sublayers.begin(), super->sublayers.end(), shared_from_this()), super->sublayers.end());
}

void CALayer::refreshGroupingFBO() {
    if (!allowsGroupOpacity || opacity >= 1 || opacity < 0.001f) {
        GPU_FreeImage(groupingFBO);
        groupingFBO = nullptr;
        return;
    }

    auto currentTarget = GPU_GetActiveTarget();

    if (!groupingFBO || groupingFBO->base_w != currentTarget->base_w || groupingFBO->base_h != currentTarget->base_h) {
        if (groupingFBO) { GPU_FreeImage(groupingFBO); }
        groupingFBO = GPU_CreateImage(currentTarget->base_w, currentTarget->base_h, GPU_FORMAT_RGBA);
        GPU_SetAnchor(groupingFBO, 0, 0);
        GPU_GetTarget(groupingFBO);
        GPU_SetVirtualResolution(groupingFBO->target, currentTarget->w, currentTarget->h);
        return;
    }
}

NXAffineTransform CALayer::affineTransform() {
    return NXTransform3DGetAffineTransform(transform);
}

void CALayer::setAffineTransform(NXAffineTransform transform) {
    this->transform = NXTransform3DMakeAffineTransform(transform);
}

CALayer* CALayer::copy() {
    return new CALayer(this);
}

std::shared_ptr<CAAction> CALayer::actionForKey(std::string event) {
    if (delegate) return delegate->actionForKey(event);
    return CALayer::defaultActionForKey(event);
}

std::shared_ptr<CALayer> CALayer::createPresentation() {
    auto copy = std::make_shared<CALayer>(this);
    copy->isPresentationForAnotherLayer = true;
    return copy;
}

Rect CALayer::getRenderedBoundsRelativeToAnchorPoint() {
    auto deltaFromAnchorPointToOrigin = Point(-(bounds.width() * anchorPoint.x),
                                              -(bounds.height() * anchorPoint.y));
    return Rect(deltaFromAnchorPointToOrigin, bounds.size);
}

std::shared_ptr<CABasicAnimation> CALayer::defaultActionForKey(std::string event) {
    auto animation = std::make_shared<CABasicAnimation>(event);
    animation->duration = CATransaction::animationDuration();
    return animation;
}

// MARK: - Animations
void CALayer::add(std::shared_ptr<CABasicAnimation> animation, std::string keyPath) {
    auto copy = std::make_shared<CABasicAnimation>(animation.get());
    copy->creationTime = Timer();

    // animation.fromValue is optional, set it to currently visible state if nil
    if (!copy->fromValue.has_value() && copy->keyPath.has_value()) {
        auto presentation = _presentation;
        if (!presentation) presentation = shared_from_this();

        copy->fromValue = presentation->value(keyPath);
    }

    if (copy->animationGroup)
        copy->animationGroup->queuedAnimations += 1;

    if (animations.count(keyPath) && animations[keyPath]->animationGroup)
        animations[keyPath]->animationGroup->animationDidStop(false);
    
    animations[keyPath] = copy;
}

void CALayer::removeAllAnimations() {
    animations.clear();
}

void CALayer::removeAnimation(std::string forKey) {
    animations.erase(forKey);
}

void CALayer::onWillSet(std::string keyPath) {
//    CALayer.layerTreeIsDirty = true
    auto animationKey = keyPath;


    auto animation = std::static_pointer_cast<CABasicAnimation>(actionForKey(animationKey));
    if (animation &&
        (this->hasBeenRenderedInThisPartOfOverallLayerHierarchy
            || animation->wasCreatedInUIAnimateBlock()) &&
        !this->isPresentationForAnotherLayer &&
        !CATransaction::disableActions())
    {
        add(animation, animationKey);
    }
}

void CALayer::onDidSetAnimations(bool wasEmpty) {
    if (!animations.empty() && wasEmpty) {
        UIView::layersWithAnimations.insert(shared_from_this());
        _presentation = createPresentation();
    } else if (animations.empty() && !wasEmpty) {
        _presentation = nullptr;
        UIView::layersWithAnimations.erase(shared_from_this());
    }
}

std::optional<AnimatableProperty> CALayer::value(std::string forKeyPath) {
    if (forKeyPath == "backgroundColor") return backgroundColor;
    if (forKeyPath == "opacity") return opacity;
    if (forKeyPath == "bounds") return bounds;
    if (forKeyPath == "transform") return transform;
    if (forKeyPath == "position") return position;
    if (forKeyPath == "anchorPoint") return anchorPoint;
    return std::nullopt;
}


void CALayer::animateAt(Timer currentTime) {
    auto presentation = createPresentation();

    for (auto& animation: animations) {
        auto animationProgress = animation.second->progressFor(currentTime);
        update(presentation, animation.second, animationProgress);

        if (animationProgress == 1 && animation.second->isRemovedOnCompletion) {
            if (animation.second->animationGroup)
                animation.second->animationGroup->animationDidStop(true);
            removeAnimation(animation.first);
        }
    }

    this->_presentation = animations.empty() ? nullptr : presentation;
}

void CALayer::update(std::shared_ptr<CALayer> presentation, std::shared_ptr<CABasicAnimation> animation, float progress) {
    auto keyPath = animation->keyPath;
    if (!keyPath.has_value()) return;

    if (keyPath == "backgroundColor") {
        auto start = any_optional_cast<UIColor>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<UIColor>(animation->toValue);
        if (!end.has_value()) end = this->backgroundColor;
        if (!end.has_value()) end = UIColor::clear;

        presentation->backgroundColor = start->interpolationTo(end.value(), progress);
    }
    if (keyPath == "position") {
        auto start = any_optional_cast<Point>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Point>(animation->toValue);
        if (!end.has_value()) end = this->position;

        presentation->position = start.value() + (end.value() - start.value()) * progress;
    }
    if (keyPath == "anchorPoint") {
        auto start = any_optional_cast<Point>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Point>(animation->toValue);
        if (!end.has_value()) end = this->anchorPoint;

        presentation->anchorPoint = start.value() + (end.value() - start.value()) * progress;
    }
    if (keyPath == "bounds") {
        auto start = any_optional_cast<Rect>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Rect>(animation->toValue);
        if (!end.has_value()) end = this->bounds;

        presentation->bounds = start.value() + (end.value() - start.value()) * progress;
    }
    if (keyPath == "opacity") {
        auto start = any_optional_cast<float>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<float>(animation->toValue);
        if (!end.has_value()) end = this->opacity;

        presentation->opacity = start.value() + (end.value() - start.value()) * progress;
    }
    if (keyPath == "transform") {
        auto start = any_optional_cast<NXTransform3D>(animation->fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<NXTransform3D>(animation->toValue);
        if (!end.has_value()) end = this->transform;

        presentation->transform = start.value() + (end.value() - start.value()) * progress;
    }
}

}
