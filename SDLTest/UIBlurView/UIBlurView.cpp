//
// Created by Daniil Vinogradov on 11/01/2024.
//

#include <UIBlurView/UIBlurView.hpp>
#include <UIBlurView/UIBlurLayer.hpp>
#include <Renderer/Renderer.hpp>

namespace UIKit {

std::shared_ptr<UIBlurView> UIBlurView::init() {
    return new_shared<UIBlurView>();
}

UIBlurView::UIBlurView(Rect frame): UIView(frame) {
    auto blurLayer = new_shared<UIBlurLayer>();
    layer()->addSublayer(blurLayer);
}

//std::shared_ptr<CALayer> UIBlurView::initLayer() {
//    return new_shared<UIBlurLayer>();
//}

}