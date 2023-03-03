//
//  Utils+Antialiasing.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 01.03.2023.
//

#include <glew.h>
#include <Utils/Utils.hpp>

namespace UIKit {

void setAntialiasing(bool enabled) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
}

}
