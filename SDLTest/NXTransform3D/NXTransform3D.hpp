//
//  NXTransform3D.hpp
//  NXKit
//
//  Created by Даниил Виноградов on 19.08.2022.
//

#pragma once

#include "NXAffineTransform.hpp"
#include <SDL_gpu.h>

namespace UIKit {

struct Vector3 {
    double x, y, z;
};

struct NXTransform3D {
public:
    double m11; double m12; double m13; double m14;
    double m21; double m22; double m23; double m24;
    double m31; double m32; double m33; double m34;
    double m41; double m42; double m43; double m44;

    NXTransform3D(double m11, double m12, double m13, double m14,
                  double m21, double m22, double m23, double m24,
                  double m31, double m32, double m33, double m34,
                  double m41, double m42, double m43, double m44);
    NXTransform3D();
    NXTransform3D(float* unsafePointer);

    static NXTransform3D translationBy(double x, double y, double z);
    static NXTransform3D scaleBy(double x, double y, double z);
    static NXTransform3D scale(double factor);
    static NXTransform3D rotationBy(double angle, double x, double y, double z);

    NXTransform3D concat(const NXTransform3D& other) const;
    bool operator==(const NXTransform3D& rhs) const;
    NXTransform3D operator+(const NXTransform3D& rhs) const;
    NXTransform3D operator-(const NXTransform3D& rhs) const;
    NXTransform3D operator*(const NXTransform3D& first) const;
    NXTransform3D operator*(const double& first) const;
    NXTransform3D interpolate(const double& progress) const;
    NXTransform3D interpolateTo(const NXTransform3D& matrix, const double& progress) const;

    Vector3 transformingVector(double x, double y, double z) const;

    static const NXTransform3D identity;

    void setAsSDLgpuMatrix() const;

};

const NXTransform3D NXTransform3DIdentity = NXTransform3D(1, 0, 0, 0,
                                                          0, 1, 0, 0,
                                                          0, 0, 1, 0,
                                                          0, 0, 0, 1);

bool NXTransform3DEqualToTransform(const NXTransform3D& a, const NXTransform3D& b);

NXTransform3D NXTransform3DMakeAffineTransform(NXAffineTransform transform);
NXTransform3D CATransform3DMakeTranslation(double tx, double ty, double tz);
NXTransform3D CATransform3DMakeScale(double sx, double sy, double sz);
NXTransform3D CATransform3DMakeRotation(double angle, double x, double y, double z);
NXTransform3D CATransform3DConcat(const NXTransform3D& a, const NXTransform3D& b);
NXAffineTransform NXTransform3DGetAffineTransform(NXTransform3D t);

}
