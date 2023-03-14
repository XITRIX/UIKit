//
//  ViewBuilder.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <ViewBuilder/ViewBuilder.hpp>
#include <Tools/Tools.hpp>

namespace UIKit {

std::shared_ptr<ViewBuilder> ViewBuilder::shared() {
    if (!_shared) _shared = new_shared<ViewBuilder>();
    return _shared;
}

}

