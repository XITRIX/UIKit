//
//  UILabel.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#include <UILabel/UILabel.hpp>
#include <Renderer/Renderer.hpp>

namespace UIKit {

std::shared_ptr<UILabel> UILabel::init() {
    return new_shared<UILabel>();
}

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

void UILabel::setNumberOfLines(int numberOfLines) {
    if (_numberOfLines == numberOfLines) return;
    _numberOfLines = numberOfLines;
    setNeedsDisplay();
}

void UILabel::setTextAlignment(NSTextAlignment textAlignment) {
    if (_textAlignment == textAlignment) return;
    _textAlignment = textAlignment;
    setNeedsDisplay();
}

void UILabel::setFont(std::shared_ptr<UIFont> font) {
    if (_font == font) return;
    _font = font;
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

void UILabel::sizeToFit() {
    auto bounds = this->bounds();
    bounds.size = sizeThatFits(Size());
    setBounds(bounds);
}

bool UILabel::applyXMLAttribute(std::string name, std::string value) {
    if (UIView::applyXMLAttribute(name, value)) return true;

    if (name == "text") {
        setText(value);
        return true;
    }

    if (name == "fontSize") {
        auto size = valueToFloat(value);
        if (!size.has_value()) return false;
        font()->pointSize = size.value();
        return true;
    }

    if (name == "textAlignment") {
        auto alignment = valueToTextAlignment(value);
        if (!alignment.has_value()) return false;
        setTextAlignment(alignment.value());
        return true;
    }

    return false;
}

}
