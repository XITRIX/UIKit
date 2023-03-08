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

//    auto wrapLength = (_numberOfLines != 1) ? bounds().width() : 0;

    layer()->setContents(font->createContentsFor(std::static_pointer_cast<UILabel>(shared_from_this())));

//    auto content = Renderer::shared()->drawFBO(Size(44, 44), 2, [](auto vg) {
////        nvgFillColor(vg, UIColor::orange.nvgColor());
////        nvgRect(vg, 0, 0, 88, 88);
////        nvgFill(vg);
//
//        nvgFillColor(vg, UIColor::red.nvgColor());
//        nvgRect(vg, 22, 0, 44, 44);
//        nvgFill(vg);
//    });
//    layer()->setContents(content);
}

void UILabel::display(std::shared_ptr<CALayer> layer) {
    draw();
}

Size UILabel::sizeThatFits(Size size) {
    return font->sizeForText(_text, 999);
}

}
