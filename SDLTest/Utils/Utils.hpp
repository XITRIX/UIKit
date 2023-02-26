//
//  Utils.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.02.2023.
//

#include <memory>
#include <string>

#pragma once

namespace UIKit {

#define ptr std::shared_ptr
#define wptr std::weak_ptr

struct Utils {
    static std::string resourcePath;
};

}
