#pragma once

#include "shapes.h"

class CubicBezierCurve : public Shape
{
  private:
    Vector2 p0;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
    double length = INVALID_LENGTH;

  public:
    CubicBezierCurve();
    Vector2 nextVector(double t) { return p0.scale(std::pow(1 - t, 3)).add(p1.scale(3 * std::pow(1 - t, 2) * t))
      .add(p2.scale(3 * (1 - t) * std::pow(t, 2))).add(p3.scale(std::pow(t, 3))); }
    CubicBezierCurve rotate(double theta) { return CubicBezierCurve(p0.rotate(theta), p1.rotate(theta), p2.rotate(theta), p3.rotate(theta)); }
    CubicBezierCurve scale(double factor) { return scale(Vector2(factor)); }
    CubicBezierCurve scale(Vector2 vector) { return CubicBezierCurve(p0.scale(vector), p1.scale(vector), p2.scale(vector), p3.scale(vector)); }
    CubicBezierCurve translate(Vector2 vector) { return CubicBezierCurve(p0.translate(vector), p1.translate(vector), p2.translate(vector), p3.translate(vector)); }
    double getLength()
    {
        if (length == INVALID_LENGTH)
        {
            Vector2 zero(0, 0);
            double lengthA = Line(p0, p3).getLength();
            double lengthB = Line(p0.sub(p1), Vector2(0, 0)).getLength() + Line(p1.sub(p2), Vector2(0, 0)).getLength() + Line(p2.sub(p3), Vector2(0, 0)).getLength();
            length = (lengthA + lengthB) / 2.;
        }
        return length;
    }
    CubicBezierCurve(Vector2 p0I, Vector2 p1I, Vector2 p2I, Vector2 p3I)
    {
        p0 = p0I;
        p1 = p1I;
        p2 = p2I;
        p3 = p3I;
    }
};

class QuadraticBezierCurve : public CubicBezierCurve
{

  public:
    QuadraticBezierCurve(Vector2 p0I, Vector2 p1I, Vector2 p2I) {
      CubicBezierCurve(p0I, p0I.add(p1I.sub(p0I).scale(2.0 / 3.0)), p2I.add(p1I.sub(p2I).scale(2.0 / 3.0)), p2I);
    }
};