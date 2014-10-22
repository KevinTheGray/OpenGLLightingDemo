#pragma once

#include "Utilities.h"

struct Transform
{
  Transform()
  {
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
  }
  Vector4 translation;
  Vector4 rotation;
  Vector4 scale;
};