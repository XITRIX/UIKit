//
// Created by Daniil Vinogradov on 12/01/2024.
//

#include <UIScrollView/DecelerationTimingParameters.hpp>
#include <cmath>

namespace UIKit {

    Point DecelerationTimingParameters::destination() const {
        auto dCoeff = 1000 * log(decelerationRate);
        auto res = initialValue - initialVelocity / dCoeff;
        return res;
    }

    float DecelerationTimingParameters::duration() const {
        if (initialVelocity.magnitude() <= 0) { return 0; }

        auto dCoeff = 1000 * log(decelerationRate);
        auto res = log(-dCoeff * threshold / initialVelocity.magnitude()) / dCoeff;
        return res;
    }

    Point DecelerationTimingParameters::valueAt(float time) const {
        auto dCoeff = 1000 * log(decelerationRate);
        auto res = initialValue + initialVelocity * (pow(decelerationRate, float(1000 * time)) - 1) / dCoeff;
        return res;
    }

    float DecelerationTimingParameters::durationTo(Point value) const {
        if (value.distanceToSegment(initialValue, destination()) >= threshold) { return 0; }

        auto dCoeff = 1000 * log(decelerationRate);
        auto res = log(1.0f + dCoeff * (value - initialValue).magnitude() / initialVelocity.magnitude()) / dCoeff;
        return res;
    }

    Point DecelerationTimingParameters::velocityAt(float time) const {
        return initialVelocity * pow(decelerationRate, float(1000 * time));
    }
}