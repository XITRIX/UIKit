//
//  Renderer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 01.03.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <nanovg.h>
#include <functional>
#include <Geometry/Geometry.hpp>
#include <CGImage/CGImage.hpp>
#include <memory>

namespace UIKit {

class Renderer {
public:
    int primaryFont = 0;
    static Renderer* shared();

    void draw(std::function<void(NVGcontext*)> draw);
    std::shared_ptr<CGImage> drawFBO(Size size, float scale, std::function<void(NVGcontext*)> draw);

    NVGcontext* getContext() const { return _vg; }
private:
    static Renderer* _shared;
    Renderer();
    ~Renderer();

    NVGcontext* _vg;
};

}
