//
// Created by Daniil Vinogradov on 11/01/2024.
//

#include <UIBlurView/UIBlurLayer.hpp>
#include <Renderer/Renderer.hpp>
#include <UIBlurView/BlurShaderProgram.hpp>

namespace UIKit {
    UIBlurLayer::UIBlurLayer(): CALayer() {
        auto data = Data::fromPath("amogus.png").value();
        image = new_shared<CGImage>(data);
    }

    BlurShaderProgram* _blurShaderProgram = nullptr;
    void UIBlurLayer::render(GPU_Target *renderer) {
        if (!_blurShaderProgram) _blurShaderProgram = new BlurShaderProgram();

        GPU_FlushBlitBuffer();
        _blurShaderProgram->activate();
        _blurShaderProgram->setImage(image, getFrame());
//        Renderer::shared()->
//        Renderer::shared()->drawRectangleFilled(Rect(0, 0, 300, 30), UIColor::red, 0);

        GPU_BlitTransform(
                image->pointee,
                nullptr,
                renderer,
                30,
                30,
                0, // rotation in degrees
                1,
                1
        );

        BlurShaderProgram::deactivateAll();

    }

}
