#include "Line.h"

Line::Line(Vector2 aIn, Vector2 bIn) {
    a = aIn;
    b = bIn;
}

Line::Line(double x1, double y1, double x2, double y2) {
    a = Vector2(x1, y1);
    b = Vector2(x2, y2);
}

double Line::getLength()
{
    if (length == INVALID_LENGTH)
    {
        // Euclidean distance approximation based on octagonal boundary
        double dx = std::abs(b.x - a.x);
        double dy = std::abs(b.y - a.y);
        double min = std::min(dy, dx);
        double max = std::max(dy,dx);

        length = 0.41 * min + 0.941246 * max;
    }
    return length;
}

vector<Line> Line::pathToLines(vector<double> path)
{
    vector<Line> lines;

    Vector2 prev(path[0], path[1]);

    for (int i = 2; i < path.size(); i += 2)
    {
        Vector2 dest = Vector2(path[i], path[i + 1]);
        lines.push_back(Line(prev, dest));
        prev = dest;
    }

    return lines;
}

bool Line::operator== (Line aIn)
{
        return aIn.a == a && aIn.b == b;
}