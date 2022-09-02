#pragma once

#include "utility.h"

class Line3D
{
  private:
    Vector3 start;
    Vector3 end;

  public:
    Line3D(Vector3 startIn, Vector3 endIn) {
        start = startIn;
        end = endIn;
    }
    Vector3 getStart() { return start; }
    Vector3 getEnd() { return end; }
    bool operator==(Line3D line3D) { return start == line3D.start && end == line3D.end; }
    std::string toString() { return "Line3D{ start=" + start.toString() + ", end=" + end.toString() + " }"; }
};