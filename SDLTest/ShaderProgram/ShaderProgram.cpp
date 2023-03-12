//
//  ShaderProgram.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 28.02.2023.
//

#include "ShaderProgram.hpp"
#include <Tools/Tools.hpp>

namespace UIKit {

// MARK: - ShaderProgramUniformVariable
ShaderProgramUniformVariable::ShaderProgramUniformVariable(std::string name, Uint32 inProgramRef) {
    location = GPU_GetUniformLocation(inProgramRef, name.c_str());
    assert(location != -1 && "Couldn't find location of UniformVariable \(name)");
}

void ShaderProgramUniformVariable::set(float newValue) {
    GPU_SetUniformf(location, newValue);
}

void ShaderProgramUniformVariable::set(Rect newValue) {
    float vals[] = { newValue.minX(), newValue.minY(), newValue.height(), newValue.width() };
    GPU_SetUniformfv(location, 4, 1, vals);
}

void ShaderProgramUniformVariable::set(std::shared_ptr<CGImage> newValue) {
    int textureUnit = 1; // Texture unit 0 is the one used internally for SDL_gpu's blitting funcs
    GPU_SetShaderImage(newValue->pointee, location, textureUnit);
}

// MARK: - ShaderProgram
ShaderProgram::ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader) {
    if (GPU_GetCurrentRenderer() == nullptr) {
        printf("Error: noRenderer\n");
        exit(-1);
//        throw Error.noRenderer
    }

    programRef = GPU_LinkShaders(vertexShader->shaderRef, fragmentShader->shaderRef);

    if (programRef == 0) {
        auto errorMessage = GPU_GetShaderMessage();
        printf("Error: linkingFailed - %s\n", errorMessage);
        exit(-1);
//        throw Error.linkingFailed(reason: errorMessage)
    }

    // NOTE: This hardcodes the names of the various coordinates
    // It's possible we should allow overriding these somehow, for now it doesn't matter
    shaderBlock = GPU_LoadShaderBlock(
        programRef,
        "gpu_Vertex",
        "gpu_TexCoord",
        "gpu_Color",
        "gpu_ModelViewProjectionMatrix"
    );
}

ShaderProgram::~ShaderProgram() {
    GPU_FreeShaderProgram(programRef);
}

void ShaderProgram::activate() {
    GPU_ActivateShaderProgram(programRef, &shaderBlock);
}

void ShaderProgram::deactivateAll() {
    GPU_ActivateShaderProgram(0, NULL);
}

MaskShaderProgram* ShaderProgram::_mask = nullptr;
MaskShaderProgram* ShaderProgram::getMask() {
    if (!_mask) _mask = new MaskShaderProgram();
    return _mask;
}

// MARK: - MaskShaderProgram
MaskShaderProgram::MaskShaderProgram() :
    ShaderProgram(VertexShader::getCommon(), FragmentShader::getMaskColourWithImage()),
    maskFrame("maskFrame", programRef),
    maskTexture("maskTexture", programRef)
{ }

void MaskShaderProgram::setMaskImage(std::shared_ptr<CGImage> maskImage, Rect frame) {
    maskFrame.set(frame);
    maskTexture.set(maskImage);
}

}
