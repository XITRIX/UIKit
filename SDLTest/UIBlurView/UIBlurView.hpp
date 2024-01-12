//
// Created by Daniil Vinogradov on 11/01/2024.
//

#pragma once

#include <UIView/UIView.hpp>

namespace UIKit {

class UIBlurView: public UIView {
public:
    static std::shared_ptr<UIBlurView> init();

    explicit UIBlurView(Rect frame = Rect());

//    std::shared_ptr<CALayer> initLayer() override;
};

}