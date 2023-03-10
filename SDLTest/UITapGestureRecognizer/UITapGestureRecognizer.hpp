//
//  UITapGestureRecognizer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.03.2023.
//

#pragma once

#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class UITapGestureRecognizer: public UIGestureRecognizer {
public:
    void touchesBegan(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;
    void touchesMoved(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;
    void touchesEnded(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;

private:
    std::shared_ptr<UITouch> trackingTouch;
    Point startPoint;
};

}
