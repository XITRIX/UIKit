//
//  ViewBuilder.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <memory>

namespace UIKit {

class ViewBuilder {
public:
    std::shared_ptr<ViewBuilder> shared();
private:
    std::shared_ptr<ViewBuilder> _shared;
};

}
