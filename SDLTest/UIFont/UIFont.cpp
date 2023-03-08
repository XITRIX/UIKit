//
//  UIFont.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 07.03.2023.
//

#include <UIFont/UIFont.hpp>
#include <Utils/Utils.hpp>

namespace UIKit {

std::map<std::string, std::shared_ptr<FontRenderer>> UIFont::cachedFontFiles;

UIFont::UIFont(std::string name, float size): fontName(name), pointSize(size) {
    if (!cachedFontFiles.count(name)) {
        cachedFontFiles[name] = std::make_shared<FontRenderer>(name);
    }

    renderer = cachedFontFiles[name];
}

Size UIFont::sizeForText(std::string text, uint wrapLength) {
    return renderer->sizeForText(text, pointSize, wrapLength, lineHeight);
}

std::shared_ptr<CGImage> UIFont::createContentsFor(std::shared_ptr<UILabel> label) {
    return renderer->createContentsFor(label);
}

std::shared_ptr<UIFont> UIFont::systemFont(float size) {
    return std::make_shared<UIFont>(Utils::resourcePath + "Fonts/switch_font.ttf", size);
}

}
