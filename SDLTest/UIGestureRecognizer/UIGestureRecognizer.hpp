//
//  UIGestureRecognizer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.03.2023.
//

#pragma once

namespace UIKit {

class UIGestureRecognizer {
public:
    bool isEnabled() { return _isEnabled; }
private:
    bool _isEnabled = true;
};

}
