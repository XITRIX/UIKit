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

    UIFont(const std::string& path, float size);

    Size sizeForText(const std::string& text, uint32_t wrapLength);
    std::shared_ptr<CGImage> createContentsFor(const std::shared_ptr<UILabel>& label);

    void setPointSize(float size) { pointSize = size; }

    static std::shared_ptr<UIFont> systemFont(float size);

    
private:
    std::shared_ptr<FontRenderer> renderer;

    static std::map<std::string, std::shared_ptr<FontRenderer>> cachedFontFiles;
};

}
