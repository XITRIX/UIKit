//
//  UIFont.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 07.03.2023.
//

#pragma once

#include <FontRenderer/FontRenderer.hpp>

#include <string>
#include <memory>

namespace UIKit {

class UIFont {
public:
    const std::string fontName;

    float pointSize;
    float lineHeight = 1.f; // TODO: Implement

    UIFont(std::string path, float size);

    Size sizeForText(std::string text, uint wrapLength);
    std::shared_ptr<CGImage> createContentsFor(std::shared_ptr<UILabel> label);

    void setPointSize(float size) { pointSize = size; }

    static std::shared_ptr<UIFont> systemFont(float size);

    
private:
    std::shared_ptr<FontRenderer> renderer;

    static std::map<std::string, std::shared_ptr<FontRenderer>> cachedFontFiles;
};

}
