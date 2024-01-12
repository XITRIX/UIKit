//
// Created by Daniil Vinogradov on 12/01/2024.
//

#pragma once

#include <Geometry/Geometry.hpp>
#include <Timer/Timer.hpp>

namespace UIKit {
    struct DecelerationTimingParameters {
    public:
        Point initialValue;
        Point initialVelocity;
        float decelerationRate;
        float threshold;

        DecelerationTimingParameters(Point initialValue, Point initialVelocity, float decelerationRate, float threshold):
            initialValue(initialValue),
            initialVelocity(initialVelocity),
            decelerationRate(decelerationRate),
            threshold(threshold) { }

        [[nodiscard]] Point destination() const;
        [[nodiscard]] float duration() const;
        [[nodiscard]] Point valueAt(float time) const;


        float durationTo(Point value) const;
        Point velocityAt(float time) const;
    };
}