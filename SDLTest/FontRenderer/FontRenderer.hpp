//
//  FontRenderer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 07.03.2023.
//

#pragma once

#include <map>
#include <string>
#include <memory>
#include <CGImage/CGImage.hpp>

namespace UIKit {

class UILabel;
class FontRenderer {
public:
    static std::map<std::string, std::shared_ptr<FontRenderer>> cache;
    static void cleanupSession();
    static bool initialize();

    FontRenderer(std::string path);
    ~FontRenderer();

    std::shared_ptr<CGImage> createContentsFor(std::shared_ptr<UILabel> label);

    Size sizeForText(std::string text, float textSize, uint wrapLength, float lineHeight);
private:
    int fontFace = -1;
};

}
