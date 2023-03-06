//
//  UIImage.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIImage/UIImage.hpp>

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
    auto image = std::make_shared<CGImage>(data);
    return std::make_shared<UIImage>(image, scale);
}

}
