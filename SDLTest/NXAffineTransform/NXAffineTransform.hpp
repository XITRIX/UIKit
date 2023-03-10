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
    float m11, m12, m21, m22, tX, tY;

    NXAffineTransform();
    NXAffineTransform(float m11, float m12, float m21, float m22, float tX, float tY);

    static NXAffineTransform translationBy(float x, float y);
    static NXAffineTransform scaleBy(float x, float y);
    static NXAffineTransform scale(float factor);
    static NXAffineTransform rotationBy(float angle);

    std::optional<NXAffineTransform> inverted() const;
    bool isIdentity() const;

    static const NXAffineTransform identity;

    bool operator==(const NXAffineTransform& rhs) const;
    NXAffineTransform operator*(const NXAffineTransform& rhs) const;
};

}
