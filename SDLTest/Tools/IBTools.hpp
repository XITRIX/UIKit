//
//  IBTools.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <optional>
#include <string>
#include <yoga/Yoga.h>
#include <UIImage/UIImage.hpp>
#include <UIEdgeInsets/UIEdgeInsets.hpp>
#include <UIViewContentMode/UIViewContentMode.hpp>

namespace UIKit {

class UIColor;
enum class NSTextAlignment;

std::optional<bool> valueToBool(std::string value);
std::optional<YGValue> valueToMetric(std::string value);
std::optional<UIColor> valueToColor(std::string value);
std::optional<float> valueToFloat(std::string value);
std::optional<YGJustify> valueToJustify(std::string value);
std::optional<YGAlign> valueToAlign(std::string value);
std::optional<YGDirection> valueToDirection(std::string value);
std::optional<YGFlexDirection> valueToFlexDirection(std::string value);
std::optional<YGWrap> valueToWrap(std::string value);
std::optional<Data> valueToRes(std::string value);
std::optional<std::shared_ptr<UIImage>> valueToImage(std::string value);
std::optional<UIViewContentMode> valueToContentMode(std::string value);
std::optional<YGPositionType> valueToPositionType(std::string value);
std::optional<NSTextAlignment> valueToTextAlignment(std::string value);
std::optional<UIEdgeInsets> valueToEdgeInsets(std::string value);

}
