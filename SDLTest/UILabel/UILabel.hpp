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
    static std::shared_ptr<UILabel> init();

    UILabel();

    void draw() override;
    void display(std::shared_ptr<CALayer> layer) override;
    Size sizeThatFits(Size size) override;
    void sizeToFit() override;

    std::string text() const { return _text; }
    void setText(std::string text);

    int numberOfLines() { return _numberOfLines; }
    void setNumberOfLines(int numberOfLines);

    std::shared_ptr<UIFont> font() const { return _font; }
    void setFont(std::shared_ptr<UIFont> font);

    NSTextAlignment textAlignment() { return _textAlignment; }
    void setTextAlignment(NSTextAlignment textAlignment);

    bool applyXMLAttribute(std::string name, std::string value) override;
private:
    int _numberOfLines = 1;
    std::string _text;
    UIColor _textColor = UIColor::black;
    NSTextAlignment _textAlignment = NSTextAlignment::left;
    std::shared_ptr<UIFont> _font = UIFont::systemFont(17);

    friend class FontRenderer;
};

}
