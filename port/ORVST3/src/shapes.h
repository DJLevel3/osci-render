#pragma once

#include <vector>
#include <cmath>

using std::vector;

#include "Vector2.h"
#include "Shape.h"

class shapeHelper
{
  public:
    // static std::vector<Shape> convert(Shape shape); // not sure if this is necessary at the moment, I'll do it later if it is - DJ_Level_3
    static vector<Shape> normalize(vector<Shape> shapes);
    static vector<Shape> normalize(vector<Shape> shapes, double width, double height);
    static vector<Shape> removeOutOfBounds(vector<Shape> shapes);
    // static Vector2 maxAbsVector(vector<? extends Shape> shapes); // unused, luckily, as I don't know how I'd implement this - DJ_Level_3
    static Vector2 maxVector(vector<Shape> shapes);
    // static vector<Shape> flip(List<Shape> shapes); // unused - DJ_Level_3
    static double height(vector<Shape> shapes);
    static double width(vector<Shape> shapes);
    static vector<Shape> translate(vector<Shape> shapes, Vector2 translation);
    static vector<Shape> generatePolygram(int sides, int angleJump, Vector2 start);
    static vector<Shape> generatePolygram(int sides, int angleJump, double scale) { return generatePolygram(sides, angleJump, Vector2(scale, scale)); } // unused - DJ_Level_3
    static vector<Shape> generatePolygon(int sides, Vector2 start) { return generatePolygram(sides, 1, start); }
    static vector<Shape> generatePolygon(int sides, double scale) { return generatePolygon(sides, Vector2(scale, scale)); } // unused - DJ_Level_3
    static double totalLength(vector<Shape> shapes);

  private:
    double between(double val) { return std::min(std::max(val, -1.), 1.); }
};