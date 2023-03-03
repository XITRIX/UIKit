//
//  ShaderProgram.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 28.02.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <Shader/Shader.hpp>
#include <CGImage/CGImage.hpp>

namespace UIKit {

class MaskShaderProgram;

struct ShaderProgramUniformVariable {
    int location;

    ShaderProgramUniformVariable(std::string name, Uint32 inProgramRef);

    void set(float newValue);
    void set(Rect newValue);
    void set(std::shared_ptr<CGImage> newValue);
};

class ShaderProgram {
public:
    Uint32 programRef;

    ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader);
    ~ShaderProgram();

    void activate();

    static void deactivateAll();
    static MaskShaderProgram* getMask();

private:
    static MaskShaderProgram* _mask;
    GPU_ShaderBlock shaderBlock;
};

class MaskShaderProgram: public ShaderProgram {
public:
    MaskShaderProgram();
    void setMaskImage(std::shared_ptr<CGImage> maskImage, Rect frame);
private:
    ShaderProgramUniformVariable maskFrame;
    ShaderProgramUniformVariable maskTexture;
};

}
