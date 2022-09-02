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

Vector2 shapeHelper::maxVector(vector<Shape> shapes) {
  double maxX = -DBL_MAX;
  double maxY = -DBL_MAX;

  for (Shape shape : shapes)
  {
      Vector2 startVector = shape.nextVector(0);
      Vector2 endVector = shape.nextVector(1);

      double x = std::max(startVector.getX(), endVector.getX());
      double y = std::max(startVector.getY(), endVector.getY());

      maxX = std::max(x, maxX);
      maxY = std::max(y, maxY);
  }

  return Vector2(maxX, maxY);
}

// vector<Shape> flip(List<Shape> shapes){} // unused - DJ_Level_3

double shapeHelper::height(vector<Shape> shapes)
{
    double maxY = -DBL_MAX;
    double minY = DBL_MAX;

    std::vector<Vector2> vectors;

    for (Shape shape : shapes)
    {
        for (int i = 0; i < 4; i++)
        {
            vectors[i] = shape.nextVector(i * 1.0 / 4);
        }

        for (Vector2 vec : vectors)
        {
            maxY = std::max(maxY, vec.y);
            minY = std::min(minY, vec.y);
        }
    }

    return std::abs(maxY - minY);
}

double shapeHelper::width(vector<Shape> shapes)
{
    double maxX = -DBL_MAX;
    double minX = DBL_MAX;

    vector<Vector2> vectors;

    for (Shape shape : shapes)
    {
        for (int i = 0; i < 4; i++)
        {
            vectors[i] = shape.nextVector(i * 1.0 / 4);
        }

        for (Vector2 vec : vectors)
        {
            maxX = std::max(maxX, vec.x);
            minX = std::min(minX, vec.x);
        }
    }

    return std::abs(maxX - minX);
}


vector<Shape> shapeHelper::translate(vector<Shape> shapes, Vector2 translation) {
    vector<Shape> translatedShapes;

    for (Shape shape : shapes)
    {
        translatedShapes.push_back(shape.translate(translation));
    }

    return translatedShapes;
}

vector<Shape> shapeHelper::generatePolygram(int sides, int angleJump, Vector2 start)
{
    vector<Shape> polygon;

    double theta = angleJump * 2 * M_PI / sides;
    Vector2 rotated = start.rotate(theta);
    polygon.push_back(Line(start, rotated));

    while (rotated != start)
    {
        polygon.push_back(Line(rotated.copy(), rotated.rotate(theta)));

        rotated = rotated.rotate(theta);
    }

    return polygon;
}