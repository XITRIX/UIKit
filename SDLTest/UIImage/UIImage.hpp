//
//  UIImage.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <CGImage/CGImage.hpp>
#include <UIRenderer/UIRenderer.hpp>

namespace UIKit {

class UIImage {
public:
    UIImage(std::shared_ptr<CGImage> cgImage, float scale);

    static std::shared_ptr<UIImage> fromPath(std::string path);
    static std::shared_ptr<UIImage> fromData(Data data, float scale = UIRenderer::main()->scale());

    std::shared_ptr<CGImage> cgImage() { return _cgImage; }
    Size size() const { return _size; }
    float scale() const { return _scale; }

private:
    std::shared_ptr<CGImage> _cgImage;
    Size _size;
    float _scale;
};

}
