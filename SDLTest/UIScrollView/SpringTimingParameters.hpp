//
// Created by Даниил Виноградов on 12.01.2024.
//

#pragma once

#include <Geometry/Geometry.hpp>
#include <Tools/SharedBase.hpp>

namespace UIKit {
    struct TimingParameters {
        virtual float duration() const { return 0; }
        virtual Point valueAt(float time) const { return {}; }
    };

    struct Spring {
        float mass;
        float stiffness;
        float dampingRatio;

        Spring(float mass, float stiffness, float dampingRatio):
            mass(mass), stiffness(stiffness), dampingRatio(dampingRatio) {}

        float damping() const;
        float beta() const;
        float dampedNaturalFrequency() const;
    };

    struct SpringTimingParameters: TimingParameters {
        Spring spring;
        Point displacement;
        Point initialVelocity;
        float threshold;

        SpringTimingParameters(Spring spring, Point displacement, Point initialVelocity, float threshold);

        float duration() const override;
        Point valueAt(float time) const override;

    private:
        std::shared_ptr<TimingParameters> impl;
    };

}
