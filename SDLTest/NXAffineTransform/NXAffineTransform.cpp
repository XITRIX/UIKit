//
//  NXAffineTransform.cpp
//  NXKit
//
//  Created by Даниил Виноградов on 19.07.2022.
//

#include <NXAffineTransform/NXAffineTransform.hpp>
#include <NXTransform3D/NXTransform3D.hpp>

#define RAD_PER_DEG 0.017453293f

namespace UIKit {

const NXAffineTransform NXAffineTransform::identity = NXAffineTransform(1, 0, 0, 1, 0, 0);

NXAffineTransform::NXAffineTransform() {}

NXAffineTransform::NXAffineTransform(float m11, float m12, float m21, float m22, float tX, float tY):
m11(m11), m12(m12), m21(m21), m22(m22), tX(tX), tY(tY)
{ }

NXAffineTransform NXAffineTransform::translationBy(float x, float y) {
    return NXAffineTransform(0, 0,
                             0, 0,
                             x, y);
}

NXAffineTransform NXAffineTransform::scaleBy(float x, float y) {
    return NXAffineTransform(x, 0,
                             0, y,
                             0, 0);
}

NXAffineTransform NXAffineTransform::scale(float f) {
    return NXAffineTransform(f, 0,
                             0, f,
                             0, 0);
}

NXAffineTransform NXAffineTransform::rotationBy(float angle) {
    float radians = angle * RAD_PER_DEG;
    float c = cosf(radians);
    float s = sinf(radians);

    return NXAffineTransform(c,  s,
                             -s, c,
                             0,  0);
}

std::optional<NXAffineTransform> NXAffineTransform::inverted() const {
    auto d = m11 * m22 - m12 * m21;
    if (d < 0) return std::nullopt;

    NXAffineTransform transform;
    float multiplyer = 1 / d;

    transform.m11 = m22 * multiplyer;
    transform.m12 = -m12 * multiplyer;
    transform.m21 = -m21 * multiplyer;
    transform.m22 = m11 * multiplyer;

    return transform;
}

bool NXAffineTransform::isIdentity() const {
    return *this == identity;
}

bool NXAffineTransform::operator==(const NXAffineTransform& rhs) const {
    return m11 == rhs.m11 &&
            m12 == rhs.m12 &&
            m21 == rhs.m21 &&
            m22 == rhs.m22 &&
            tX == rhs.tX &&
            tY == rhs.tY;
}

NXAffineTransform NXAffineTransform::operator*(const NXAffineTransform& rhb) const {
    auto a = NXTransform3DMakeAffineTransform(*this);
    auto b = NXTransform3DMakeAffineTransform(rhb);
    return NXTransform3DGetAffineTransform(a * b);
}

}
