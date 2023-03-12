//
//  UILabel.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#pragma once

#include <UIView/UIView.hpp>
#include <UIFont/UIFont.hpp>

namespace UIKit {

enum class NSTextAlignment {
    center,
    left,
    right,
};

class UILabel: public UIView {
public:
    UILabel();

    void draw() override;
    void display(std::shared_ptr<CALayer> layer) override;
    Size sizeThatFits(Size size) override;

    void layoutSubviews() override;

    void setText(std::string text);
    std::string text() const { return _text; }

    std::shared_ptr<UIFont> font() const { return _font; }
    void setFont(std::shared_ptr<UIFont> font) { _font = font; }
private:
    int _numberOfLines = 1;
    std::string _text;
    UIColor _textColor = UIColor::black;
    NSTextAlignment _textAlignment = NSTextAlignment::left;
    std::shared_ptr<UIFont> _font = UIFont::systemFont(17);

    friend class FontRenderer;
};

}
