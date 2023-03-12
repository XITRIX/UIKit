//
//  Shader.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 28.02.2023.
//

#include <Shader/Shader.hpp>
#include <Tools/Tools.hpp>
#include <string>

namespace UIKit {

Shader::Shader(std::string source, GPU_ShaderEnum type) {
    auto renderer = GPU_GetCurrentRenderer();
    if (!renderer) {
        printf("Error: noRenderer\n");
        exit(-1);
//        throw Error.noRenderer;
    }

    std::string header;
    switch (renderer->shader_language) {
        case GPU_LANGUAGE_GLSL: {
            header = "#version " + std::to_string(renderer->max_shader_version);
            break;
        }
        case GPU_LANGUAGE_GLSLES: {
            header =
            "precision highp int;"
            "precision highp float;";
            break;
        }
        default: {
            printf("Error: unknownRendererShaderLanguage\n");
            exit(-1);
//           throw Error.unknownRendererShaderLanguage
        }
    }

    std::string fullShaderSource = header + "\n" + source;

    shaderRef = GPU_CompileShader(type, fullShaderSource.c_str());

    if (shaderRef == 0) {
        auto errorMessage = GPU_GetShaderMessage();
        printf("Error: compilationFailed - \n%s\n", errorMessage);
         exit(-1);
//        throw Error.compilationFailed(reason: errorMessage)
    }
}

Shader::~Shader() {
    GPU_FreeShader(shaderRef);
}

#ifdef SDL_GPU_DISABLE_GLES
char* VertexShader::in = "in";
char* VertexShader::out = "out";
#elif SDL_GPU_DISABLE_OPENGL
char* VertexShader::in = "attribute";
char* VertexShader::out = "varying";
#endif


VertexShader::VertexShader(std::string source):
    Shader(source, GPU_VERTEX_SHADER)
{ }


VertexShader* VertexShader::_common = nullptr;
VertexShader* VertexShader::getCommon() {
    if (!_common) _common = new VertexShader(
             string_sprintf("%s vec3 gpu_Vertex;"
                            "%s vec2 gpu_TexCoord;"
                            "%s vec4 gpu_Color;"
                            "uniform mat4 gpu_ModelViewProjectionMatrix;"

                            "%s vec4 originalColour;"
                            "%s vec2 absolutePixelPos;"

                            "void main(void)"
                            "{"
                            "originalColour = gpu_Color;"
                            "absolutePixelPos = vec2(gpu_Vertex.xy);"
                            "gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 1.0);"
                            "}", VertexShader::in, VertexShader::in, VertexShader::in, VertexShader::out, VertexShader::out)
                                             );
    return _common;
}

#ifdef SDL_GPU_DISABLE_GLES
char* FragmentShader::in = "in";
char* FragmentShader::texture = "texture";
char* FragmentShader::fragColor = "fragColor";
char* FragmentShader::fragColorDefinition = "out vec4 fragColor;";
#elif SDL_GPU_DISABLE_OPENGL
char* FragmentShader::in = "varying";
char* FragmentShader::texture = "texture2D";
char* FragmentShader::fragColor = "gl_FragColor";
char* FragmentShader::fragColorDefinition = ""; // magically predefined by the driver
#endif


FragmentShader::FragmentShader(std::string source):
    Shader(source, GPU_FRAGMENT_SHADER)
{ }

FragmentShader* FragmentShader::_maskColourWithImage = nullptr;
FragmentShader* FragmentShader::getMaskColourWithImage() {
    if (!_maskColourWithImage) {
        _maskColourWithImage = new FragmentShader(
                   string_sprintf("%s vec4 originalColour;"
                                  "%s vec2 absolutePixelPos;"

                                  "%s"

                                  "uniform vec4 maskFrame;"
                                  "uniform sampler2D maskTexture;"

                                  "void main(void)"
                                  "{"
                                  "vec2 maskCoordinate = vec2("
                                  "((absolutePixelPos.x - maskFrame.x) / maskFrame.w),"
                                  "((absolutePixelPos.y - maskFrame.y) / maskFrame.z)"
                                  ");"

                                  "vec4 maskColour = %s(maskTexture, maskCoordinate);"
                                  "%s = vec4(originalColour.rgb, originalColour.a * maskColour.a);"
                                  "}", FragmentShader::in, FragmentShader::in,
                                  FragmentShader::fragColorDefinition,
                                  FragmentShader::texture,
                                  FragmentShader::fragColor)
                                );
    }
    return _maskColourWithImage;
}

}
