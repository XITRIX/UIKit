//
// Created by Daniil Vinogradov on 11/01/2024.
//

#include <UIBlurView/BlurShaderProgram.hpp>
#include <Tools/Tools.hpp>
#include <utility>

namespace UIKit {

    FragmentShader* _blurShader = nullptr;

    FragmentShader* getBlurShader() {
        if (!_blurShader) {
            _blurShader = new FragmentShader(string_sprintf("%s vec4 originalColour;"
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
                                                            //
                                                            "vec4 maskColour = %s(maskTexture, maskCoordinate);"
                                                            "%s = maskColour;"//vec4(1, 1, 0, originalColour.a);"
                                                            "}", FragmentShader::in, FragmentShader::in,
                                                            FragmentShader::fragColorDefinition,
                                                            FragmentShader::texture,
                                                            FragmentShader::fragColor));
        }
        return _blurShader;
    }

    BlurShaderProgram::BlurShaderProgram(): ShaderProgram(VertexShader::getCommon(), getBlurShader()),
                                            maskFrame("maskFrame", programRef),
                                            maskTexture("maskTexture", programRef) {}


    void BlurShaderProgram::setImage(std::shared_ptr<CGImage> image, Rect frame) {
        maskFrame.set(frame);
        maskTexture.set(std::move(image));
    }
}
