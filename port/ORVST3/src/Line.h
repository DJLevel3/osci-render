#pragma once

#include "shapes.h"

class Line : public Shape
{
  private:
    Vector2 a;
    Vector2 b;
    double length = INVALID_LENGTH;

  public:
    Line(Vector2 a, Vector2 b);
    Line(double x1, double y1, double x2, double y2);
    Line rotate(double theta) { return Line(a.rotate(theta), b.rotate(theta)); }
    Line translate(Vector2 vector) { return Line(a.translate(vector), b.translate(vector)); }
    double getLength();
    Line scale(double factor) { return Line(a.scale(factor), b.scale(factor)); }
    Line scale(Vector2 vector) { return Line(a.scale(vector), b.scale(vector)); }
    Line copy() { return Line(a.copy(), b.copy()); }
    Vector2 nextVector(double drawingProgress) { return Vector2(a.x + drawingProgress * (b.x - a.x), a.y + drawingProgress * (b.y - a.y)); }
    Vector2 getA() { return a; }
    Vector2 getB() { return b; }
    double getX1() { return a.getX(); }
    double getY1() { return a.getY(); }
    double getX2() { return b.getX(); }
    double getY2() { return b.getY(); }
    Line setX1(double x1) { return Line(x1, getY1(), getX2(), getY2()); }
    Line setY1(double y1) { return Line(getX1(), y1, getX2(), getY2()); }
    Line setX2(double x2) { return Line(getX1(), getY1(), x2, getY2()); }
    Line setY2(double y2) { return Line(getX1(), getY1(), getX2(), y2); }
    static vector<Line> pathToLines(vector<double> path);
    bool operator==(Line aIn);
    std::string toString() { return "Line{ a=" + a.toString() + ", b=" + b.toString() + " }"; }
};