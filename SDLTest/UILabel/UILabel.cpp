//
//  UILabel.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#include <UILabel/UILabel.hpp>
#include <Renderer/Renderer.hpp>

namespace UIKit {


UILabel::UILabel(): UIView() {
//    setContentMode(UIViewContentMode::center);
    setContentMode(UIViewContentMode::left);
}

void UILabel::setText(std::string text) {
    if (_text == text) return ;
    _text = text;

//    auto size = font->sizeForText(_text, bounds().size.width);
//    printf("Size: %f, %f", size.width, size.height);
    
    setNeedsDisplay();
}

void UILabel::draw() {
    UIView::draw();
    layer()->setContents(_font->createContentsFor(std::static_pointer_cast<UILabel>(shared_from_this())));
}

void UILabel::display(std::shared_ptr<CALayer> layer) {
    draw();
}

Size UILabel::sizeThatFits(Size size) {
    return _font->sizeForText(_text, size.width);
}

void UILabel::layoutSubviews() {
    UIView::layoutSubviews();
    setNeedsDisplay();
}

}
