#pragma once
#include "utility.h"

class CameraDrawKernel
{
  private:
    const int MIN_GPU_VERTICES = 50;
    WorldObject prevObject;
  public:
    CameraDrawKernel();
};