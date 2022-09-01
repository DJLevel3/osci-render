#pragma once

#include "shapes.h"

class Shape
{
  protected:
    double INVALID_LENGTH = -1;

  public:
    Vector2 nextVector(double drawingProgress);
    Shape rotate(double theta);
    Shape scale(double factor);
    Shape scale(Vector2 vector);
    Shape translate(Vector2 vector);
    double getLength();
};