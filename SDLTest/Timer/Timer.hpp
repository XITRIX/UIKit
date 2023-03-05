//
//  Timer.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#pragma once

#include <sys/time.h>

namespace UIKit {

struct Timer {
    timeval startTime;

    Timer(double startingAt = 0);

    double getElapsedTimeInMilliseconds();
    double getElapsedTimeInSeconds();

    double operator-(const Timer& rhs) const;
};

}
