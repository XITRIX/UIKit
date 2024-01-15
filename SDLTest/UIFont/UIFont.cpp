//
//  UIFont.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 07.03.2023.
//

#include <UIFont/UIFont.hpp>
#include <Tools/Tools.hpp>

namespace UIKit {

std::map<std::string, std::shared_ptr<FontRenderer>> UIFont::cachedFontFiles;

UIFont::UIFont(const std::string& name, float size): fontName(name), pointSize(size) {
    if (!cachedFontFiles.count(name)) {
        cachedFontFiles[name] = new_shared<FontRenderer>(name);
    }

    renderer = cachedFontFiles[name];
}

Size UIFont::sizeForText(const std::string& text, uint wrapLength) {
    return renderer->sizeForText(text, pointSize, wrapLength, lineHeight);
}

std::shared_ptr<CGImage> UIFont::createContentsFor(const std::shared_ptr<UILabel>& label) {
    return renderer->createContentsFor(label);
}

std::shared_ptr<UIFont> UIFont::systemFont(float size) {
//    return new_shared<UIFont>(Utils::resourcePath + "Fonts/switch_font.ttf", size);
    return new_shared<UIFont>("Fonts/SFPro.ttf", size);
}

}
