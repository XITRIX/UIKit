//
// Created by Daniil Vinogradov on 11/01/2024.
//

#pragma once

#include <CALayer/CALayer.hpp>

namespace UIKit {

    class UIBlurLayer: public CALayer {
    public:
        UIBlurLayer();
        void render(GPU_Target *renderer) override;
    private:
        std::shared_ptr<CGImage> image;
    };

}
