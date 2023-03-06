//
//  UIResponder.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <memory>

namespace UIKit {

class UIResponder {
public:
    std::weak_ptr<UIResponder> next;
};

}
