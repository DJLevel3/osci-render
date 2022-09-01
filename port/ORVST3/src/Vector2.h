#pragma once

#include "shapes.h"

class Vector2 : public Shape
{
  public:
    Vector2(double x, double y);
    double getX();
    double getY();
};