//
//  Fatal.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <Tools/Fatal.hpp>

namespace UIKit {

[[noreturn]] void fatal(std::string message)
{
//    brls::Logger::error("Fatal error: {}", message);
    printf("%s\n", message.c_str());
    throw std::logic_error(message);
}

}
