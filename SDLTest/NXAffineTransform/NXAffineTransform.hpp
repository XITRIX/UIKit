//
//  NXAffineTransform.hpp
//  NXKit
//
//  Created by Даниил Виноградов on 19.07.2022.
//

#pragma once

#include <optional>

namespace UIKit {

struct NXAffineTransform {
public:
    double m11, m12, m21, m22, tX, tY;

    NXAffineTransform();
    NXAffineTransform(double m11, double m12, double m21, double m22, double tX, double tY);

    static NXAffineTransform translationBy(double x, double y);
    static NXAffineTransform scaleBy(double x, double y);
    static NXAffineTransform scale(double factor);
    static NXAffineTransform rotationBy(double angle);

    std::optional<NXAffineTransform> inverted() const;
    bool isIdentity() const;

    static const NXAffineTransform identity;

    bool operator==(const NXAffineTransform& rhs) const;
    NXAffineTransform operator*(const NXAffineTransform& rhs) const;
};

}
