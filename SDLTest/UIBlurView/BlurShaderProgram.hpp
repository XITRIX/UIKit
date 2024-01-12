//
// Created by Daniil Vinogradov on 11/01/2024.
//

#pragma once

#include <ShaderProgram/ShaderProgram.hpp>

namespace UIKit {

class BlurShaderProgram: public ShaderProgram {
public:
    BlurShaderProgram();
    void setImage(std::shared_ptr<CGImage> image, Rect frame);
private:
        ShaderProgramUniformVariable maskFrame;
        ShaderProgramUniformVariable maskTexture;
};

//    class MaskShaderProgram: public ShaderProgram {
//    public:
//        MaskShaderProgram();
//        void setMaskImage(std::shared_ptr<CGImage> maskImage, Rect frame);
//    private:
//        ShaderProgramUniformVariable maskFrame;
//        ShaderProgramUniformVariable maskTexture;
//    };

}
