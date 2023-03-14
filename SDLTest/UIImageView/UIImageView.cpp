//
//  UIImageView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIImageView/UIImageView.hpp>
#include <Renderer/Renderer.hpp>

namespace UIKit {

std::shared_ptr<UIImageView> UIImageView::init() {
    return new_shared<UIImageView>();
}

UIImageView::UIImageView(std::shared_ptr<UIImage> image): UIImageView(Rect()) {
    _image = image;
    updateTextureFromImage();
}

UIImageView::UIImageView(Rect frame): UIView(frame) {
    isUserInteractionEnabled = false;
}

void UIImageView::setImage(std::shared_ptr<UIImage> image) {
    if (image == _image) { return; }
    _image = image;

    updateTextureFromImage();
    setNeedsLayout();
}

void UIImageView::updateTextureFromImage() {
    if (_image) {
        layer()->setContents(_image->cgImage());
        layer()->contentsScale = _image->scale();

        auto bounds = this->bounds();
        bounds.size = _image->size();
        setBounds(bounds);
    } else {
        layer()->setContents(nullptr);
        layer()->contentsScale = UIRenderer::main()->scale();
    }
}

void UIImageView::sizeToFit() {
    UIView::sizeToFit();
}

Size UIImageView::sizeThatFits(Size size) {
    if (!_image) return Size();
    return _image->size();
}

bool UIImageView::applyXMLAttribute(std::string name, std::string value) {
    if (UIView::applyXMLAttribute(name, value)) return true;

    if (name == "image") {
        auto image = valueToImage(value);
        if (!image.has_value()) return false;
        setImage(image.value());
        return true;
    }

    return false;
}

}
