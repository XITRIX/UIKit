//
//  Global.h
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL2/SDL.h>

namespace UIKit {

class UIApplication {
public:
    static SDL_Renderer* currentRenderer;
};

}
