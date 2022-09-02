#pragma once

#include "shapes.h"

class Vector2 : public Shape
{
  public:
    double x = 0;
    double y = 0;

    Vector2(double xIn, double yIn) { x = xIn; y = yIn; }
    Vector2(double xy) { x = xy; y = xy; }
    Vector2();
    double getX() { return x; };
    double getY() { return y; };
    Vector2 setX(double newX) { return Vector2(newX, y); };
    Vector2 setY(double newY) { return Vector2(x, newY); };
    Vector2 copy() { return Vector2(x, y); }
    Vector2 add(Vector2 vector) { return translate(vector); }
    Vector2 sub(Vector2 vector) { return Vector2(getX() - vector.getX(), getY() - vector.getY()); }
    Vector2 reflectRelativeToVector(Vector2 vector) { return translate(vector.sub(*this).scale(2)); }
    Vector2 nextVector(double drawingProgress) { return *this; }
    Vector2 scale(double factor) { return scale(Vector2(factor)); }
    Vector2 scale(Vector2 vector) { return Vector2(x * vector.x, y * vector.y); }
    Vector2 translate(Vector2 vector) { return Vector2(x + vector.x, y + vector.y); }
    double getLength() { return 0; }
    bool operator==(Vector2 a) { return x == a.x && y == a.y; }
    bool operator!=(Vector2 a) { return x != a.x || y != a.y; }
    std::string toString() { return "Vector2{ x=" + std::to_string(x) + ", y=" + std::to_string(y) + " }"; }
    Vector2 rotate(double theta)
    {
        double cosTheta = std::cos(theta);
        double sinTheta = std::sin(theta);
        return Vector2(x * cosTheta - y * sinTheta, x * sinTheta + y * cosTheta);
    }
};