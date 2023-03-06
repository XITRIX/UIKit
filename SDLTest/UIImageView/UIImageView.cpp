//
//  UIImageView.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIImageView/UIImageView.hpp>

namespace UIKit {

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

Size UIImageView::sizeThatFits(Size size) {
    if (!_image) return Size();
    return _image->size();
}

}
