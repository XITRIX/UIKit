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

Point Point::applying(NXAffineTransform t) {
    return Point(x * t.m11 + y * t.m21 + t.tX,
                 x * t.m12 + y * t.m22 + t.tY);

}

Point Point::zero = Point();

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

Size Size::operator*(const float& rhs) const {
    return Size(width * rhs, height * rhs);
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

void Rect::setWidth(float newValue) { size.width = newValue; }
void Rect::setHeight(float newValue) { size.height = newValue; }

void Rect::setMinX(float newValue) { origin.x = newValue; }
void Rect::setMidX(float newValue) { origin.x = newValue - (size.width / 2); }
void Rect::setMaxX(float newValue) { origin.x = newValue - size.width; }

void Rect::setMinY(float newValue) { origin.y = newValue; }
void Rect::setMidY(float newValue) { origin.y = newValue - (size.height / 2); }
void Rect::setMaxY(float newValue) { origin.y = newValue - size.height; }

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

Rect Rect::operator+(const Rect& rhs) const {
    return Rect(
        this->origin.x + rhs.origin.x,
        this->origin.y + rhs.origin.y,
        this->size.width + rhs.size.width,
        this->size.height + rhs.size.height
    );
}

Rect Rect::operator-(const Rect& rhs) const {
    return Rect(
        this->origin.x - rhs.origin.x,
        this->origin.y - rhs.origin.y,
        this->size.width - rhs.size.width,
        this->size.height - rhs.size.height
    );
}

Rect Rect::operator*(const float& rhs) const {
    return Rect(
        this->origin.x * rhs,
        this->origin.y * rhs,
        this->size.width * rhs,
        this->size.height * rhs
    );
}

Rect Rect::applying(NXAffineTransform t) {
    if (t.isIdentity()) { return *this; }

    auto newTopLeft = Point(minX(), minY()).applying(t);
    auto newTopRight = Point(maxX(), minY()).applying(t);
    auto newBottomLeft = Point(minX(), maxY()).applying(t);
    auto newBottomRight = Point(maxX(), maxY()).applying(t);

    auto newMinX = min(newTopLeft.x, newTopRight.x, newBottomLeft.x, newBottomRight.x);
    auto newMaxX = max(newTopLeft.x, newTopRight.x, newBottomLeft.x, newBottomRight.x);

    auto newMinY = min(newTopLeft.y, newTopRight.y, newBottomLeft.y, newBottomRight.y);
    auto newMaxY = max(newTopLeft.y, newTopRight.y, newBottomLeft.y, newBottomRight.y);

    // XXX: What happens if the point that was furthest left is now on the right (because of a rotation)?
    // i.e. Should do we return a normalised rect or one with a negative width?
    return Rect(newMinX,
                newMinY,
                newMaxX - newMinX,
                newMaxY - newMinY);
}

Rect Rect::applying(NXTransform3D t) {
    if (t == NXTransform3DIdentity) { return *this; }

    auto topLeft = t.transformingVector(minX(), minY(), 0);
    auto topRight = t.transformingVector(maxX(), minY(), 0);
    auto bottomLeft = t.transformingVector(minX(), maxY(), 0);
    auto bottomRight = t.transformingVector(maxX(), maxY(), 0);

    auto newMinX = min(topLeft.x, topRight.x, bottomLeft.x, bottomRight.x);
    auto newMaxX = max(topLeft.x, topRight.x, bottomLeft.x, bottomRight.x);

    auto newMinY = min(topLeft.y, topRight.y, bottomLeft.y, bottomRight.y);
    auto newMaxY = max(topLeft.y, topRight.y, bottomLeft.y, bottomRight.y);

    return Rect(newMinX, newMinY, newMaxX - newMinX, newMaxY - newMinY);
}

Rect Rect::intersection(Rect other) const {
    auto largestMinX = fmaxf(minX(), other.minX());
    auto largestMinY = fmaxf(minY(), other.minY());

    auto smallestMaxX = fmaxf(maxX(), other.maxX());
    auto smallestMaxY = fmaxf(maxY(), other.maxY());

    auto width = smallestMaxX - largestMinX;
    auto height = smallestMaxY - largestMinY;

    if (width > 0 && height > 0) {
        // The intersection rectangle has dimensions, i.e. there is an intersection:
        return Rect(largestMinX, largestMinY, width, height);
    } else {
        return null;
    }
}

GPU_Rect Rect::gpuRect() const {
    return GPU_MakeRect(origin.x, origin.y, size.width, size.height);
}

bool Rect::isNull() const {
    return *this == null;
}

Rect Rect::null = Rect(INFINITY, INFINITY, 0, 0);

}
