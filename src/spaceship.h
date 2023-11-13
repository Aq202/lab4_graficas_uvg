#pragma once
#include "model.h"

class SpaceshipModel : public Model
{
public:
  SpaceshipModel(Camera &camera) : Model("../models/nave.obj", camera, 0.05, 5.0f)
  {
    setRotationY(180.0f);
  }

  Fragment fragmentShader(Fragment& fragment) {
   
   Color color = Color(255,255,0);

   float intensity = fragment.intensity;

   if (intensity < 0.3) intensity = 0.3;

    fragment.color = color * fragment.intensity;

    return fragment;
}

glm::vec3 calculateLightDirection() override
  {

    return glm::vec3(0.0f, 1.0f, 1.0f);
  }

void forward(float distance = 0.5f)
  {
    float rotation = (getRotationY()  - 180.0f) * 3.1416f/180.0f;
    glm::vec3 forwardVector = glm::vec3(distance * std::sin(rotation), 0.0f, distance * std::cos(rotation));
    glm::vec3 currentPosition = getCurrentPosition();
    setTranslation(currentPosition + forwardVector);

  }

  void up(float distance = 0.5f)
  {
    glm::vec3 upVector = glm::vec3(0.0f, distance, 0.0f);
    glm::vec3 currentPosition = getCurrentPosition();
    setTranslation(currentPosition + upVector);

  }

};