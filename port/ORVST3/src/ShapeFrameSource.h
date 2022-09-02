#pragma once

using std::vector;

#include "utility.h"

class ShapeFrameSource
{
  private:
    vector<Shape> shapes;
    bool active = true;

  public:
    ShapeFrameSource();
    ShapeFrameSource(vector<Shape> inShapes) { shapes = inShapes; }
    vector<Shape> next() { return shapes; }
    bool isActive() { return active; }
    void disable() { active = false; }
    void enable() { active = true; }
};