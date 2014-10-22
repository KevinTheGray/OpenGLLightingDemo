#pragma once
#include "Mesh.h"
#include "Transform.h"

class Object
{
public:
  Mesh *mesh;
  Transform trans;

  Object()
  {
    mesh = 0;
  }
};