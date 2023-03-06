//
//  UIImageView.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIView/UIView.hpp>
#include <UIImage/UIImage.hpp>

namespace UIKit {

class UIImageView: public UIView {
public:
    UIImageView(std::shared_ptr<UIImage> image = nullptr);
    UIImageView(Rect frame);

    void setImage(std::shared_ptr<UIImage> image);
    std::shared_ptr<UIImage> image() { return _image; }

    Size sizeThatFits(Size size) override;

private:
    std::shared_ptr<UIImage> _image;
    void updateTextureFromImage();
};

}
