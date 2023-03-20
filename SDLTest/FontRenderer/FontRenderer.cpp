//
//  FontRenderer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 07.03.2023.
//

#include <FontRenderer/FontRenderer.hpp>
#include <Renderer/Renderer.hpp>
#include <UILabel/UILabel.hpp>
#include <Tools/Tools.hpp>
#include <nanovg.h>

namespace UIKit {

std::map<std::string, std::shared_ptr<FontRenderer>> FontRenderer::cache;

void FontRenderer::cleanupSession() {
    cache.clear();
}

bool FontRenderer::initialize() {
    return true;
}

FontRenderer::FontRenderer(std::string path) {
    fontFace = nvgCreateFont(Renderer::shared()->getContext(), path.c_str(), path.c_str());
}

FontRenderer::~FontRenderer() { }

std::shared_ptr<CGImage> FontRenderer::createContentsFor(std::shared_ptr<UILabel> label) {
    auto size = label->bounds().size;
    float scale = UIRenderer::main()->scale();

    NVGalign align;
    switch (label->textAlignment()) {
        case NSTextAlignment::center: {
            align = NVG_ALIGN_CENTER;
            break;
        }
        case NSTextAlignment::left: {
            align = NVG_ALIGN_LEFT;
            break;
        }
        case NSTextAlignment::right: {
            align = NVG_ALIGN_RIGHT;
            break;
        }
    }

    return Renderer::shared()->drawFBO(size, scale, [this, size, label, align](auto vg) {
        nvgFontSize(vg, label->_font->pointSize);
        nvgTextAlign(vg, align | NVG_ALIGN_TOP);
        nvgFontFaceId(vg, fontFace);
        nvgTextLineHeight(vg, label->_font->lineHeight);
        nvgFillColor(vg, label->textColor().nvgColor());
        
        nvgTextBox(vg, 0, 0, size.width, label->_text.c_str(), nullptr);
    });
}

// Optimise perf
int newLineR = '\r';
int newLineN = '\n';

bool charIsDelimiter(char ch) {
    auto wrappingDelimiters = " -\t\r\n";
    return strchr(wrappingDelimiters, ch) != nullptr;
}

Size FontRenderer::sizeForText(std::string text, float textSize, uint wrapLength, float lineHeight) {
    auto vg = Renderer::shared()->getContext();
    
    // Setup nvg state for the measurements
    nvgFontSize(vg, textSize);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFontFaceId(vg, fontFace);
    nvgTextLineHeight(vg, lineHeight);
    nvgFillColor(vg, UIColor::black.nvgColor());

    // Measure the needed width for the fullText
    float bounds[4];
    nvgTextBoxBounds(vg, 0, 0, wrapLength == 0 ? MAXFLOAT : wrapLength, text.c_str(), nullptr, bounds);

    float requiredWidth = bounds[2] - bounds[0];
    float requiredHeight = bounds[3] - bounds[1];

    return Size(requiredWidth + 6, requiredHeight);




//    const int lineSpace = 2;
//    int textLineHeight = 0;
//
//    char* tok = (char*) text.c_str();
//    char* end = tok + SDL_strlen(tok);
//
//    std::vector<char*> lines;
//
//    /* Get the longest string that will fit in the desired space */
//    do {
//        lines.push_back(tok);
//
//        /* Look for the end of the line */
//
//        char* searchIndex = strchr(tok, newLineR);
//        if (!searchIndex) searchIndex = strchr(tok, newLineN);
//        if (!searchIndex) searchIndex = end;
//
//        auto firstCharOfNextLine = searchIndex + 1;
//
//        /* Get the longest string that will fit in the desired space */
//        while (true) {
//            /* Strip trailing whitespace */
//            while (searchIndex > tok && charIsDelimiter(searchIndex[-1])) {
//                searchIndex -= 1;
//            }
//
//            if (searchIndex == tok) {
//                if (charIsDelimiter(*searchIndex)) {
//                    *searchIndex = 0;
//                }
//                break;
//            }
//
//            auto delim = *searchIndex;
//            *searchIndex = 0;
//
//            int textLineWidth = 0;
//            nvgTextBou
//            TTF_SizeUTF8(self.rawPointer, tok, &textLineWidth, &textLineHeight);
//
//            if (UInt32(textLineWidth) <= wrapLength) {
//                break
//            } else {
//                /* Back up and try again... */
//                searchIndex.pointee = delim
//            }
//
//            while searchIndex > tok && !charIsDelimiter(searchIndex[-1]) {
//                searchIndex -= 1
//            }
//
//            if searchIndex > tok {
//                firstCharOfNextLine = searchIndex
//            }
//        }
//
//    } while(tok < end);
//
//
//    return Size();
}


}
