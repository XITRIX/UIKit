//
//  Geometry.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <NXAffineTransform/NXAffineTransform.hpp>
#include <NXTransform3D/NXTransform3D.hpp>

namespace UIKit {

struct Point {
    float x, y;

    Point();
    Point(float x, float y);

    bool operator==(const Point& rhs) const;
    Point operator+(const Point& first) const;
    Point operator-(const Point& first) const;
    Point& operator+=(const Point& rhs);
    Point& operator-=(const Point& rhs);
    Point operator/(const float& rhs);
    Point operator*(const float& rhs);

    Point applying(NXAffineTransform transform);

    static Point zero;
};

struct Size {
    float width, height;

    Size();
    Size(float width, float height);

    bool operator==(const Size& rhs) const;
    Size operator+(const Size& first) const;
    Size operator-(const Size& first) const;
    Size& operator+=(const Size& rhs);
    Size& operator-=(const Size& rhs);

    Size operator*(const float& rhs) const;
};

struct Rect {
    Point origin;
    Size size;

    Rect();
    Rect(Point origin, Size size);
    Rect(float x, float y, float width, float height);

    float width() const;
    float height() const;

    float minX() const;
    float midX() const;
    float maxX() const;

    float minY() const;
    float midY() const;
    float maxY() const;

    void setWidth(float newValue);
    void setHeight(float newValue);

    void setMinX(float newValue);
    void setMidX(float newValue);
    void setMaxX(float newValue);

    void setMinY(float newValue);
    void setMidY(float newValue);
    void setMaxY(float newValue);

    bool contains(Point point);
    bool intersects(const Rect& other) const;
    Rect& offsetBy(const Point& offset);
    Rect& offsetBy(const float& offsetX, const float& offsetY);

    bool operator==(const Rect& rhs) const;
    Rect operator+(const Rect& rhs) const;
    Rect operator-(const Rect& rhs) const;
    Rect operator*(const float& rhs) const;

    Rect applying(NXAffineTransform transform);
    Rect applying(NXTransform3D transform);

    GPU_Rect gpuRect() const;
};

}
