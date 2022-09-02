#pragma once

#include "utility.h"

class Vector3
{
  public:
    double x, y, z;
    Vector3(double xIn, double yIn, double zIn) {
        x = xIn;
        y = yIn;
        z = zIn;
    }
    Vector3(double xyz=0) { Vector3(xyz, xyz, xyz); }
    Vector3 add(Vector3 other) { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 sub(Vector3 other) { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 scale(double factor) { return Vector3(x * factor, y * factor, z * factor); }
    Vector3 rotate(Vector3 rotation) { return rotateX(rotation.x).rotateY(rotation.y).rotateZ(rotation.z); }
    Vector3 rotateX(double theta)
    {
        double cos = std::cos(theta);
        double sin = std::sin(theta);
        return Vector3(x, cos * y - sin * z, sin * y + cos * z);
    }
    Vector3 rotateY(double theta)
    {
        double cos = std::cos(theta);
        double sin = std::sin(theta);
        return Vector3(cos * x + sin * z, y, -sin * x + cos * z);
    }
    Vector3 rotateZ(double theta)
    {
        double cos = std::cos(theta);
        double sin = std::sin(theta);
        return Vector3(cos * x - sin * y, sin * x + cos * y, z);
    }
    double distance(Vector3 vector)
    {
        double xDiff = x - vector.x;
        double yDiff = y - vector.y;
        double zDiff = z - vector.z;
        return std::sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
    }
    static Vector3 meanPoint(vector<Vector3> points)
    {
        Vector3 mean = Vector3();

        for (Vector3 point : points)
        {
            mean = mean.add(point);
        }

        return mean.scale(1. / (points.size()));
    }
    bool operator==(Vector3 obj) { return x == obj.x && y == obj.y && z == obj.z; }
    Vector3 clone() { return Vector3(x, y, z); }
    std::string toString() {return "Vector3{ x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z) + " }"; }
};