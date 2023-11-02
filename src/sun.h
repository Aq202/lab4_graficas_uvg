#pragma once
#include "model.h"

class SunModel : public Model
{
public:
  SunModel(Camera &camera) : Model("../models/sphere.obj", camera, glm::vec3(0.5f, 0.0f, 0.0f))
  {
  }

  Fragment fragmentShader(Fragment &fragment)
  {
    Color color;

    float noise = rand() % 100;

    if (noise < 10)
      color = Color(78, 0, 0);
    else if (noise < 30)
      color = Color(251, 186, 98);
    else if (noise < 40)
      color = Color(252, 100, 0);
    else if (noise < 50)
      color = Color(239, 144, 55);
    else
      color = Color(207, 24, 3);

    fragment.color = color * fragment.intensity;

    return fragment;
  }
};