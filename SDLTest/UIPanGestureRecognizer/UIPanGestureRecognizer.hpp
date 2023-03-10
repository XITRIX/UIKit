//
//  UIPanGestureRecognizer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.03.2023.
//

#pragma once

#include <UIGestureRecognizer/UIGestureRecognizer.hpp>
#include <Geometry/Geometry.hpp>

namespace UIKit {

class UIPanGestureRecognizer: public UIGestureRecognizer {
public:
    void touchesBegan(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;
    void touchesMoved(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;
    void touchesEnded(std::vector<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;

    Point translationInView(std::shared_ptr<UIView> view);
    void setTranslation(Point translation, std::shared_ptr<UIView> inView);

    Point velocityIn(std::shared_ptr<UIView> view);

private:
    std::shared_ptr<UITouch> trackingTouch;
    Point initialTouchPoint;
    time_t touchesMovedTimestamp;
    time_t previousTouchesMovedTimestamp;

    Point velocityIn(std::shared_ptr<UIView> view, float timeDiffSeconds);
};

}
