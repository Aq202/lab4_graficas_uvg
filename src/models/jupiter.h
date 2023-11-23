#pragma once
#include "../model.h"

class JupiterModel : public Model
{
public:
  JupiterModel(Camera &camera) : Model("../models/sphere.obj", camera, 1.5f, 35.0f)
  {
  }

  Fragment fragmentShader(Fragment& fragment) {
    

    const float y = fragment.original.y;

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    float offset_x = 10000.0f;
    float offset_y = 15000.0f;
    float zoom = 200.0f;

    float noise = noiseGenerator.GetNoise((uv.x + offset_x) * zoom, (uv.y + offset_y) * zoom);

    //SDL_Log("%f", noise);

    glm::vec3 tempColor;

    if(y > 0.3) tempColor = glm::vec3(148.0f, 133.0f, 118.0f);
    else if(y > 0.26) tempColor = glm::vec3(232.0f ,238.0f ,240.0f);
    else if(y > 0.23) tempColor = glm::vec3(119.0f, 100.0f, 86.0f);
    else if(y > 0.19) tempColor = glm::vec3(232.0f, 238.0f, 240.0f);
    else if (y > 0.05) tempColor = glm::vec3(151.0f, 133.0f, 121.0f);
    else if (y > -0.05) tempColor = glm::vec3(187.0f, 180.0f, 179.0f);
    else if (y > -0.16) tempColor = glm::vec3(117.0f, 101.0f, 89.0f);
    else if (y > -0.25) tempColor = glm::vec3(154.0f, 156.0f, 169.0f);
    else if (y > -0.35) tempColor = glm::vec3(127.0f, 123.0f, 123.0f);
    else tempColor = glm::vec3(56.0f, 50.0f, 47.0f);

    if(noise > 0.9) tempColor *= 0.8f;
    else if (noise > 0.6 && noise < 0.65) tempColor *= 1.06f;

    Color color = Color(tempColor.x, tempColor.y, tempColor.z);

    fragment.color = color * fragment.intensity;

    return fragment;
}

void render() override{
    Model::render();
    Orbit orbit = Orbit(getUniforms(), getRadius(), Color(0.0f, 255.0f, 0.0f));
    orbit.render();
  }

};