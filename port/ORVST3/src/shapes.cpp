#include "shapes.h"

vector<Shape> shapeHelper::normalize(vector<Shape> shapes) {
    double oldHeight = height(shapes);
    double oldWidth = width(shapes);
    double maxDim = std::max(oldHeight, oldWidth);
    vector<Shape> normalizedShapes;

    for (Shape shape : shapes)
    {
        normalizedShapes.push_back(shape.scale(Vector2(2 / maxDim, -2 / maxDim)));
    }

    Vector2 maxVec = maxVector(normalizedShapes);
}

vector<Shape> shapeHelper::normalize(vector<Shape> shapes, double width, double height)
{
    double maxDim = std::max(width, height);
    vector<Shape> normalizedShapes;

    for (Shape shape : shapes)
    {
        normalizedShapes.push_back(shape.scale(Vector2(2 / maxDim, -2 / maxDim)));
    }

    normalizedShapes = translate(normalizedShapes, Vector2(-1, 1));
    return removeOutOfBounds(normalizedShapes);
}

vector<Shape> shapeHelper::removeOutOfBounds(vector<Shape> shapes)
{
    vector<Shape> culledShapes;

    for (Shape shape : shapes)
    {
        Vector2 start = shape.nextVector(0);
        Vector2 end = shape.nextVector(1);
        if ((start.getX() < 1 && start.getX() > -1) || (start.getY() < 1 && start.getY() > -1))
        {
            if ((end.getX() < 1 && end.getX() > -1) || (end.getY() < 1 && end.getY() > -1)) {
                  culledShapes.push_back(shape);
            }
        }
    }
    return culledShapes;
}

// Vector2 maxAbsVector(vector<? extends Shape> shapes){} // unused, luckily, as I don't know how I'd implement this - DJ_Level_3
Vector2 shapeHelper::maxVector(vector<Shape> shapes) {}
// vector<Shape> flip(List<Shape> shapes){} // unused - DJ_Level_3
double shapeHelper::height(vector<Shape> shapes) {}
double shapeHelper::width(vector<Shape> shapes) {}
vector<Shape> shapeHelper::translate(vector<Shape> shapes, Vector2 translation) {}
vector<Shape> shapeHelper::generatePolygram(int sides, int angleJump, Vector2 start) {}