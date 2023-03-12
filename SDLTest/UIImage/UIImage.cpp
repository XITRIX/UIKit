//
//  UIImage.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIImage/UIImage.hpp>
#include <Tools/Tools.hpp>

namespace UIKit {

UIImage::UIImage(std::shared_ptr<CGImage> cgImage, float scale):
    _cgImage(cgImage), _scale(scale), _size(cgImage->size())
{ }

std::shared_ptr<UIImage> UIImage::fromPath(std::string path) {
    auto imageData = Data::fromPath(path);
    if (!imageData.has_value()) { return nullptr; }
    return fromData(imageData.value());
}

std::shared_ptr<UIImage> UIImage::fromData(Data data, float scale) {
    auto image = new_shared<CGImage>(data);
    return new_shared<UIImage>(image, scale);
}

}
