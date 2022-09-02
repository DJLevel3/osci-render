#pragma once
#include "utility.h"

class Camera
{
  private:
    const int MAX_NUM_STEPS = 1000;
    const double DEFAULT_FOCAL_LENGTH = 1.;
    const double VERTEX_VALUE_THRESHOLD = 1.;
    const double CAMERA_MOVE_INCREMENT = -0.1;
    const int SAMPLE_RENDER_SAMPLES = 50;
    const int VERTEX_SAMPLES = 1000;

  public:
    CameraDrawKernel kernel;
};