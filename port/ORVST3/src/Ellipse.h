#pragma once
#include "shapes.h"

class Ellipse : public Shape
{
  private:
    double a;
    double b;
    double rotation;
    Vector2 position;
    double length = INVALID_LENGTH;

  public:
    Ellipse(double aIn, double bIn, double rotationIn, Vector2 positionIn);
    Ellipse(double a, double b, Vector2 position) { Ellipse(a, b, 0, position); }
    Ellipse(double a, double b) { Ellipse(a, b, 0, Vector2()); }
    Vector2 nextVector(double drawingProgress);
    Ellipse rotate(double theta) { return Ellipse(a, b, fmod(theta + rotation, 2 * M_PI), position); }
    Ellipse scale(double factor) { return scale(Vector2(factor)); }
    Ellipse scale(Vector2 vector) { return Ellipse(a * vector.getX(), b * vector.getY(), rotation, position.scale(vector)); }
    Ellipse translate(Vector2 vector) { return Ellipse(a, b, rotation, position.translate(vector)); }
    double getLength();
};