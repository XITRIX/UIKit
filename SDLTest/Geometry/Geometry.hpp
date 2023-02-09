//
//  Geometry.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#pragma once

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

    bool contains(Point point);
    bool intersects(const Rect& other) const;
    Rect& offsetBy(const Point& offset);
    Rect& offsetBy(const float& offsetX, const float& offsetY);

    bool operator==(const Rect& rhs) const;
};

}
