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
bool CALayer::layerTreeIsDirty = true;

CALayer::CALayer() { }

CALayer::CALayer(CALayer* layer) {
    delegate = layer->delegate;
    _bounds = layer->_bounds;
    _transform = layer->_transform;
    _position = layer->_position;
    _anchorPoint = layer->_anchorPoint;
    _opacity = layer->_opacity;
    _backgroundColor = layer->_backgroundColor;
    _isHidden = layer->_isHidden;
    _cornerRadius = layer->_cornerRadius;
//    borderWidth = layer->borderWidth;
//    borderColor = layer->borderColor;
//    shadowColor = layer->shadowColor;
//    shadowPath = layer->shadowPath;
//    shadowOffset = layer->shadowOffset;
//    shadowRadius = layer->shadowRadius;
//    shadowOpacity = layer->shadowOpacity;
    mask = layer->mask;
    _masksToBounds = layer->_masksToBounds;
    _contents = layer->_contents; // XXX: we should make a copy here
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

    if (_isHidden || _opacity < 0.001f) { return; }

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
    auto translationToPosition = CATransform3DMakeTranslation(_position.x, _position.y, zPosition);
    auto transformAtPositionInParentCoordinates = parentOriginTransform * translationToPosition;

    auto modelViewTransform = transformAtPositionInParentCoordinates * _transform;

    // Now that we're in our own coordinate system based around `anchorPoint` (which is generally the middle of
    // bounds.size), we need to find the top left of the rectangle in order to be able to render rectangles.
    // Since we have already applied our own `transform`, we can work in our own (`bounds.size`) units.
    auto deltaFromAnchorPointToOrigin = Point(-(_bounds.width() * _anchorPoint.x),
                                              -(_bounds.height() * _anchorPoint.y));
    auto renderedBoundsRelativeToAnchorPoint = Rect(deltaFromAnchorPointToOrigin, _bounds.size);

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
        deltaFromAnchorPointToOrigin.x - _bounds.origin.x,
        deltaFromAnchorPointToOrigin.y - _bounds.origin.y,
        0 // If we moved (e.g.) forward to render `self`, all sublayers should start at that same zIndex
    );

    // This transform is referred to as the `parentOriginTransform` in our sublayers (see above):
    auto transformAtSelfOrigin = modelViewTransform * translationFromAnchorPointToOrigin;

    // MARK: Masking / clipping rect
    auto screenRenderer = UIRenderer::main();
    auto previousClippingRect = screenRenderer->clippingRect();

    if (_masksToBounds) {
        // If a previous clippingRect exists restrict it further, otherwise just set it:
        if (previousClippingRect.has_value()) {
            screenRenderer->setClippingRect(previousClippingRect.value().intersection(absoluteFrame));
        } else {
            screenRenderer->setClippingRect(absoluteFrame);
        }
    }

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
        mask->presentationOrSelf()->render(maskFBO->pointee->target);

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
    if (_backgroundColor.has_value()) {
        if (_cornerRadius <= 0.001f) {
            GPU_RectangleFilled2(localRenderer, renderedBoundsRelativeToAnchorPoint.gpuRect(), _backgroundColor.value().color);
        } else {
            Renderer::shared()->draw([this, renderedBoundsRelativeToAnchorPoint](auto context) {
                auto color = _backgroundColor.value().color;
                nvgBeginPath(context);
                nvgFillColor(context, nvgRGBA(color.r, color.g, color.b, color.a));
                nvgRoundedRect(context, renderedBoundsRelativeToAnchorPoint.minX(), renderedBoundsRelativeToAnchorPoint.minY(), renderedBoundsRelativeToAnchorPoint.width(), renderedBoundsRelativeToAnchorPoint.height(), _cornerRadius);
                nvgFill(context);
            });
        }
    }

    // Contents
    if (_contents) {
        auto contentsGravity = ContentsGravityTransformation(this);
        GPU_SetAnchor(_contents->pointee, _anchorPoint.x, _anchorPoint.y);
        GPU_SetRGBA(_contents->pointee, 255, 255, 255, _opacity * 255);

        GPU_BlitTransform(
            _contents->pointee,
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
    
//    Renderer::shared()->draw([this](auto context) {
//        draw(context);
//    });

    // Apply transform for subviews
    transformAtSelfOrigin.setAsSDLgpuMatrix();
    for (auto sublayer: sublayers) {
        sublayer->presentationOrSelf()->render(localRenderer);
    }

    parentOriginTransform.setAsSDLgpuMatrix();

    if (groupingFBO) {
        GPU_SetActiveTarget(renderer);
        GPU_SetRGBA(groupingFBO, 255, 255, 255, _opacity * 255);
//        GPU_Blit(groupingFBO, nullptr, renderer, 0, 0);
        auto rect = GPU_MakeRect(0, 0, renderer->w, renderer->h);
        GPU_BlitRect(groupingFBO, NULL, renderer, &rect);
    }

    screenRenderer->setClippingRect(previousClippingRect);

}

Rect CALayer::getFrame() {
    // Create a rectangle based on `bounds.size` * `transform` at `position` offset by `anchorPoint`
    auto transformedBounds = _bounds.applying(_transform);

    auto anchorPointOffset = Point(
        transformedBounds.width() * _anchorPoint.x,
        transformedBounds.height() * _anchorPoint.y
                                   );

    return Rect(_position.x - anchorPointOffset.x,
                _position.y - anchorPointOffset.y,
                transformedBounds.width(),
                transformedBounds.height());
}

void CALayer::setFrame(Rect frame) {
    setPosition(Point(frame.origin.x + (frame.width() * _anchorPoint.x),
                     frame.origin.y + (frame.height() * _anchorPoint.y)));

    auto inverseTransformOpt = affineTransform().inverted();
    if (!inverseTransformOpt.has_value()) {
//        assertionFailure("You tried to set the frame of a CALayer whose transform cannot be inverted. This is undefined behaviour.");
        return;
    }
    auto inverseTransform = inverseTransformOpt.value();


    // If we are shrinking the view with a transform and then setting a
    // new frame, the layer's actual `bounds` is bigger (and vice-versa):
    auto nonTransformedBoundSize = frame.applying(inverseTransform).size;

    auto bounds = _bounds;
    bounds.size = nonTransformedBoundSize;
    setBounds(bounds);
}

void CALayer::setAnchorPoint(Point anchorPoint) {
    if (_anchorPoint == anchorPoint) return;
    onWillSet("anchorPoint");
    _anchorPoint = anchorPoint;
}

void CALayer::setPosition(Point position) {
    if (_position == position) return;
    onWillSet("position");
    _position = position;
}

void CALayer::setBounds(Rect bounds) {
    if (_bounds == bounds) return;
    onWillSet("bounds");
    _bounds = bounds;
}

void CALayer::setOpacity(float opacity) {
    if (_opacity == opacity) return;
    onWillSet("opacity");
    _opacity = opacity;
}

void CALayer::setTransform(NXTransform3D transform) {
    if (_transform == transform) return;
    onWillSet("transform");
    _transform = transform;
}

void CALayer::setCornerRadius(float cornerRadius) {
    if (_cornerRadius == cornerRadius) return;
    onWillSet("cornerRadius");
    _cornerRadius = cornerRadius;
}

void CALayer::setBackgroundColor(std::optional<UIColor> backgroundColor) {
    if (_backgroundColor == backgroundColor) return;
    onWillSet("backgroundColor");
    _backgroundColor = backgroundColor;
}

void CALayer::setMask(std::shared_ptr<CALayer> mask) {
    if (this->mask) {
        this->mask->superlayer.reset();
    }
    this->mask = mask;
    if (mask) mask->superlayer = shared_from_this();
}

void CALayer::setHidden(bool hidden) {
    _isHidden = hidden;
    CALayer::layerTreeIsDirty = true;
}

void CALayer::setContents(std::shared_ptr<CGImage> contents) {
    _contents = contents;
    CALayer::layerTreeIsDirty = true;
}

std::shared_ptr<CALayer> CALayer::getMask() const {
    return mask;
}

void CALayer::addSublayer(std::shared_ptr<CALayer> layer) {
    layer->removeFromSuperlayer();
    sublayers.push_back(layer);
    layer->superlayer = this->shared_from_this();
    CALayer::layerTreeIsDirty = true;
}

void CALayer::insertSublayerAt(std::shared_ptr<CALayer> layer, int index) {
    layer->removeFromSuperlayer();
    sublayers.insert(sublayers.begin() + index, layer);
    layer->superlayer = this->shared_from_this();
    CALayer::layerTreeIsDirty = true;
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
    CALayer::layerTreeIsDirty = true;
}

void CALayer::refreshGroupingFBO() {
    if (!allowsGroupOpacity || _opacity >= 1 || _opacity < 0.001f) {
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
    return NXTransform3DGetAffineTransform(_transform);
}

void CALayer::setAffineTransform(NXAffineTransform transform) {
    this->setTransform(NXTransform3DMakeAffineTransform(transform));
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

void CALayer::display() {
    if (delegate) delegate->display(shared_from_this());
}

Rect CALayer::getRenderedBoundsRelativeToAnchorPoint() {
    auto deltaFromAnchorPointToOrigin = Point(-(_bounds.width() * _anchorPoint.x),
                                              -(_bounds.height() * _anchorPoint.y));
    return Rect(deltaFromAnchorPointToOrigin, _bounds.size);
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

    auto isEmpty = animations.empty();
    animations[keyPath] = copy;
    onDidSetAnimations(isEmpty);
}

void CALayer::removeAllAnimations() {
    auto isEmpty = animations.empty();
    animations.clear();
    onDidSetAnimations(isEmpty);
}

void CALayer::removeAnimation(std::string forKey) {
    auto isEmpty = animations.empty();
    animations.erase(forKey);
    onDidSetAnimations(isEmpty);
}

void CALayer::onWillSet(std::string keyPath) {
    CALayer::layerTreeIsDirty = true;
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
    if (forKeyPath == "backgroundColor") return _backgroundColor;
    if (forKeyPath == "opacity") return _opacity;
    if (forKeyPath == "bounds") return _bounds;
    if (forKeyPath == "transform") return _transform;
    if (forKeyPath == "position") return _position;
    if (forKeyPath == "anchorPoint") return _anchorPoint;
    if (forKeyPath == "cornerRadius") return _cornerRadius;
    return std::nullopt;
}

std::shared_ptr<CALayer> CALayer::presentationOrSelf() {
    if (_presentation) return _presentation;
    return shared_from_this();
}

void CALayer::animateAt(Timer currentTime) {
    auto presentation = createPresentation();

    auto animationsCopy = animations;
    for (auto& animation: animationsCopy) {
        auto animationProgress = animation.second->progressFor(currentTime);
        update(presentation, animation.second, animationProgress);

        if (animationProgress == 1 && animation.second->isRemovedOnCompletion) {
            removeAnimation(animation.first);
            if (animation.second->animationGroup)
                animation.second->animationGroup->animationDidStop(true);
        }
    }

    this->_presentation = animations.empty() ? nullptr : presentation;
}

// Writing into `presentation->_...` cause we don't need onWillSet to be triggered
void CALayer::update(std::shared_ptr<CALayer> presentation, std::shared_ptr<CABasicAnimation> animation, float progress) {
    if (!animation->keyPath.has_value() || !animation->fromValue.has_value()) return;

    auto keyPath = animation->keyPath.value();
    auto fromValue = animation->fromValue.value();

    if (keyPath == "backgroundColor") {
        auto start = any_optional_cast<std::optional<UIColor>>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<std::optional<UIColor>>(animation->toValue);
        if (!end.has_value()) end = this->_backgroundColor;
        if (!end.has_value()) end = UIColor::clear;

        presentation->setBackgroundColor(start.value()->interpolationTo(end.value().value(), progress));
    }
    if (keyPath == "position") {
        auto start = any_optional_cast<Point>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Point>(animation->toValue);
        if (!end.has_value()) end = this->_position;

        presentation->setPosition(start.value() + (end.value() - start.value()) * progress);
    }
    if (keyPath == "anchorPoint") {
        auto start = any_optional_cast<Point>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Point>(animation->toValue);
        if (!end.has_value()) end = this->_anchorPoint;

        presentation->setAnchorPoint(start.value() + (end.value() - start.value()) * progress);
    }
    if (keyPath == "bounds") {
        auto start = any_optional_cast<Rect>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<Rect>(animation->toValue);
        if (!end.has_value()) end = this->_bounds;

        presentation->setBounds(start.value() + (end.value() - start.value()) * progress);
    }
    if (keyPath == "opacity") {
        auto start = any_optional_cast<float>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<float>(animation->toValue);
        if (!end.has_value()) end = this->_opacity;

        presentation->setOpacity(start.value() + (end.value() - start.value()) * progress);
    }
    if (keyPath == "cornerRadius") {
        auto start = any_optional_cast<float>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<float>(animation->toValue);
        if (!end.has_value()) end = this->_cornerRadius;

        presentation->setCornerRadius(start.value() + (end.value() - start.value()) * progress);
    }
    if (keyPath == "transform") {
        auto start = any_optional_cast<NXTransform3D>(fromValue);
        if (!start.has_value()) { return; }

        auto end = any_optional_cast<NXTransform3D>(animation->toValue);
        if (!end.has_value()) end = this->_transform;

//        presentation->_transform = start.value() + (end.value() - start.value()) * progress;
//        presentation->_transform = start.value() + (end.value() - start.value()).interpolate(progress);
        presentation->setTransform(start.value().interpolateTo(end.value(), progress));
    }
}

}
