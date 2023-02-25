//
//  Runner.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 21.02.2023.
//

#include <SDL2/SDL.h>
#include <SDL_gpu.h>

class Runner {
public:
    int startApp();
private:
    GPU_Target* renderer;
    void refreshScreenResolution(Uint16 width, Uint16 height);
};
