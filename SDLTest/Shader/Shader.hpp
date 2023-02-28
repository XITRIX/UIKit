//
//  Shader.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 28.02.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <string>

namespace UIKit {

class Shader {
public:
    Uint32 shaderRef;

    Shader(std::string source, GPU_ShaderEnum type);
    ~Shader();
};

class VertexShader: public Shader {
public:
    static char* in;
    static char* out;

    VertexShader(std::string source);

    static VertexShader* getCommon();
private:
    static VertexShader* _common;

};

class FragmentShader: public Shader {
public:
    static char* in;
    static char* texture;
    static char* fragColor;
    static char* fragColorDefinition;

    FragmentShader(std::string source);

    static FragmentShader* getMaskColourWithImage();
private:
    static FragmentShader* _maskColourWithImage;
};

}
