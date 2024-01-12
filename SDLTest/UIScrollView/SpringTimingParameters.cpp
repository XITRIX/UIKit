//
// Created by Даниил Виноградов on 12.01.2024.
//

#include <UIScrollView//SpringTimingParameters.hpp>

namespace UIKit {
    struct UnderdampedSpringTimingParameters: public TimingParameters {
        Spring spring;
        Point displacement;
        Point initialVelocity;
        float threshold;

        UnderdampedSpringTimingParameters(Spring spring, Point displacement, Point initialVelocity, float threshold):
            spring(spring),
            displacement(displacement),
            initialVelocity(initialVelocity),
            threshold(threshold) {}

        Point c2() const {
            return (initialVelocity + displacement * spring.beta() ) / spring.dampedNaturalFrequency();
        }

        float duration() const override {
            if (displacement.magnitude() == 0 && initialVelocity.magnitude() == 0) {
                return 0;
            }

            return float(log((displacement.magnitude() + c2().magnitude()) / threshold) / spring.beta());
        }

        Point valueAt(float t) const override {
            auto wd = spring.dampedNaturalFrequency();
            return (displacement * cos(wd * t) + c2() * sin(wd * t)) * exp(-spring.beta() * t);
        }
    };

    struct CriticallyDampedSpringTimingParameters: public TimingParameters {
        Spring spring;
        Point displacement;
        Point initialVelocity;
        float threshold;

        CriticallyDampedSpringTimingParameters(Spring spring, Point displacement, Point initialVelocity, float threshold):
            spring(spring),
            displacement(displacement),
            initialVelocity(initialVelocity),
            threshold(threshold) {}

        Point c2() const {
            return initialVelocity + displacement * spring.beta();
        }

        float duration() const override {
            if (displacement.magnitude() == 0 && initialVelocity.magnitude() == 0) {
                return 0;
            }

            auto b = spring.beta();
            auto e = float(M_E);

            auto t1 = 1 / b * log(2 * displacement.magnitude() / threshold);
            auto t2 = 2 / b * log(4 * c2().magnitude() / (e * b * threshold));

            return fmaxf(t1, t2);
        }

        Point valueAt(float t) const override {
            return (displacement + c2() * t) * exp(-spring.beta() * t);
        }
    };

    float Spring::damping() const {
        return 2 * dampingRatio * sqrt(mass * stiffness);
    }

    float Spring::beta() const {
        return damping() / (2 * mass);
    }

    float Spring::dampedNaturalFrequency() const {
        return sqrt(stiffness / mass) * sqrt(1 - dampingRatio * dampingRatio);
    }

    SpringTimingParameters::SpringTimingParameters(Spring spring, Point displacement, Point initialVelocity, float threshold):
        spring(spring), displacement(displacement), initialVelocity(initialVelocity), threshold(threshold)
    {
        if (spring.dampingRatio == 1) {
            impl = std::make_shared<CriticallyDampedSpringTimingParameters>(spring,
                    displacement,
                    initialVelocity,
                    threshold);
        } else if (spring.dampingRatio > 0 && spring.dampingRatio < 1) {
            impl = std::make_shared<UnderdampedSpringTimingParameters>(spring,
                    displacement,
                    initialVelocity,
                    threshold);
        } else {
            printf("dampingRatio should be greater than 0 and less than or equal to 1");
        }
    }

    float SpringTimingParameters::duration() const {
        auto res = impl->duration();
        return res;
    }

    Point SpringTimingParameters::valueAt(float time) const {
        auto res = impl->valueAt(time);
        return res;
    }
}
