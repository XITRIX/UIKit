//
//  Fatal.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <string>
#include <stdexcept>

namespace UIKit {

/**
 * Prints the given error message message and throws a std::logic_error.
 */
[[noreturn]] void fatal(std::string message);

}
