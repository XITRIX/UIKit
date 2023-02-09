//
//  Geometry.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <Geometry/Geometry.hpp>
#include <math.h>

namespace UIKit {

// MARK: - PRIVATE -
float min(float a, float b, float c, float d) {
    auto minValue = (a < b) ? a : b;
    minValue = (minValue < c) ? minValue : c;
    minValue = (minValue < d) ? minValue : d;
    return minValue;
}

float max(float a, float b, float c, float d) {
    auto maxValue = (a > b) ? a : b;
    maxValue = (maxValue > c) ? maxValue : c;
    maxValue = (maxValue > d) ? maxValue : d;
    return maxValue;
}

float isEqual(float val1, float val2) {
    if (isnan(val1) && isnan(val2))
        return true;
    return val1 == val2;
}

// MARK: - POINT -
Point::Point(): x(0), y(0) { }
Point::Point(float x, float y): x(x), y(y) { }

bool Point::operator==(const Point& rhs) const {
    return this->x == rhs.x && this->y == rhs.y;
}

Point Point::operator+(const Point& first) const {
    return Point(x + first.x, y + first.y);
}

Point Point::operator-(const Point& first) const {
    return Point(x - first.x, y - first.y);
}

Point& Point::operator+=(const Point& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Point& Point::operator-=(const Point& rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

Point Point::operator/(const float& rhs) {
    auto res = *this;
    res.x /= rhs;
    res.y /= rhs;
    return res;
}

Point Point::operator*(const float& rhs) {
    auto res = *this;
    res.x *= rhs;
    res.y *= rhs;
    return res;
}

// MARK: - SIZE -
Size::Size(): width(0), height(0) { }
Size::Size(float width, float height): width(width), height(height) { }

bool Size::operator==(const Size& rhs) const {
    return isEqual(this->width, rhs.width) && isEqual(this->height, rhs.height);
}

Size Size::operator+(const Size& first) const {
    return Size(width + first.width, height + first.height);
}

Size Size::operator-(const Size& first) const {
    return Size(width - first.width, height - first.height);
}

Size& Size::operator+=(const Size& rhs) {
    this->width += rhs.width;
    this->height += rhs.height;
    return *this;
}

Size& Size::operator-=(const Size& rhs) {
    this->width -= rhs.width;
    this->height -= rhs.height;
    return *this;
}

// MARK: - RECT -
Rect::Rect(): origin(), size() { }
Rect::Rect(Point origin, Size size): origin(origin), size(size) { }
Rect::Rect(float x, float y, float width, float height): origin(x, y), size(width, height) { }

float Rect::width() const { return size.width; }
float Rect::height() const { return size.height; }

float Rect::minX() const { return origin.x; }
float Rect::midX() const { return origin.x + size.width / 2; }
float Rect::maxX() const { return origin.x + size.width; }

float Rect::minY() const { return origin.y; }
float Rect::midY() const { return origin.y + size.height / 2; }
float Rect::maxY() const { return origin.y + size.height; }

bool Rect::contains(Point point) {
    return
    (point.x >= minX()) && (point.x < maxX()) &&
    (point.y >= minY()) && (point.y < maxY());
}

bool Rect::intersects(const Rect& other) const {
    return !((minX() > other.maxX() || maxX() < other.minX()) || (minY() > other.maxY() || maxY() < other.minY()));
}

Rect& Rect::offsetBy(const Point& offset) {
    origin.x += offset.x;
    origin.y += offset.y;
    return *this;
}

Rect& Rect::offsetBy(const float& offsetX, const float& offsetY) {
    origin.x += offsetX;
    origin.y += offsetY;
    return *this;
}

bool Rect::operator==(const Rect& rhs) const {
    return
    this->origin.x == rhs.origin.x && this->origin.y == rhs.origin.y &&
    this->size.width == rhs.size.width && this->size.height == rhs.size.height;
}

}
