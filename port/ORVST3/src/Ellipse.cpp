#include "Ellipse.h"

Ellipse::Ellipse(double aIn, double bIn, double rotationIn, Vector2 positionIn)
{
    a = aIn;
    b = bIn;
    rotation = rotationIn;
    position = positionIn;
}

Vector2 Ellipse::nextVector(double drawingProgress) {
    double theta = 2 * M_PI * drawingProgress;
    return position.add( Vector2(a * std::cos(theta) * std::cos(rotation) - b * std::sin(theta) * std::sin(rotation), a * std::cos(theta) * std::sin(rotation) + b * std::sin(theta) * std::cos(rotation)));
}

double Ellipse::getLength()
{
    if (length == INVALID_LENGTH)
    {
        // Approximation of length.
        length = 2 * M_PI * std::sqrt((a * a + b * b) / 2);
    }
    return length;
}