//
//  UIColor.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <UIColor/UIColor.hpp>

namespace UIKit {

UIColor UIColor::clear = UIColor(0, 0, 0, 0);
UIColor UIColor::red = UIColor(255, 0, 0);
UIColor UIColor::green = UIColor(0, 255, 0);
UIColor UIColor::blue = UIColor(0, 0, 255);
UIColor UIColor::orange = UIColor(255, 150, 0);
UIColor UIColor::cyan = UIColor(0, 150, 255);
UIColor UIColor::white = UIColor(255, 255, 255);
UIColor UIColor::black = UIColor(0, 0, 0);
UIColor UIColor::gray = UIColor(155, 155, 155);

UIColor UIColor::separator = UIColor(208, 208, 208);
UIColor UIColor::systemBackground = UIColor(235, 235, 235);
UIColor UIColor::secondarySystemBackground = UIColor(240, 240, 240);
UIColor UIColor::tetriarySystemBackground = UIColor(252, 255, 248);
UIColor UIColor::systemTint = UIColor(49, 79, 235);

UIColor UIColor::withAlphaComponent(float alpha) {
    auto res = *this;
    res.color.a *= alpha;
    return res;
}

}

