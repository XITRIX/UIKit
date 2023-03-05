//
//  UIColor.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL2/SDL.h>
#include <nanovg.h>

namespace UIKit {

struct UIColor {
    SDL_Color color;
    UIColor(): UIColor(0, 0, 0, 0) {}
    UIColor(Uint8 r, Uint8 g, Uint8 b): color{r, g, b, 255} {}
    UIColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a): color{r, g, b, a} {}

    UIColor withAlphaComponent(float alpha);
    
    static UIColor clear;
    static UIColor red;
    static UIColor green;
    static UIColor blue;
    static UIColor orange;
    static UIColor cyan;
    static UIColor white;
    static UIColor black;
    static UIColor gray;

    static UIColor separator;
    static UIColor systemBackground;
    static UIColor secondarySystemBackground;
    static UIColor tetriarySystemBackground;
    static UIColor systemTint;

    NVGcolor nvgColor();

    UIColor interpolationTo(UIColor endResult, float progress);

    bool operator==(const UIColor& rhs) const;
};

}
