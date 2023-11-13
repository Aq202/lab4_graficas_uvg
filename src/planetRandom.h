#pragma once
#include "model.h"

class PlanetRandomModel : public Model
{
public:
  PlanetRandomModel(Camera &camera) : Model("../models/sphere.obj", camera, 0.5f, 27.0f)
  {
  }

  Fragment fragmentShader(Fragment& fragment) {
    Color color;

    glm::vec3 purple = glm::vec3(91.0f, 8.0f, 136.0f);
    glm::vec3 pink = glm::vec3(242.0f, 4.0f, 127.0f);
    glm::vec3 blue = glm::vec3(53.0f, 52.0f, 158.0f);
    glm::vec3 aqua = glm::vec3(24.0f, 116.0f, 132.0f);
    glm::vec3 green = glm::vec3(14.0f, 206.0f, 58.0f);
    glm::vec3 yellow = glm::vec3(221.0f, 231.0f, 6.0f);

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    float offset_x = 8000.0f;
    float offset_y = 8000.0f;
    float zoom = 200.0f;

    float noise = noiseGenerator.GetNoise((uv.x + offset_x) * zoom, (uv.y + offset_y) * zoom);
    //SDL_Log("%f", noiseValue);

    glm::vec3 tmpColor = pink;

    if(noise > 0.8) tmpColor = purple;
    else if (noise < 0.7 && noise > 0.63) tmpColor = yellow;
    else if (noise < 0.55 && noise > 0) tmpColor = green;
    else if (noise < 0 && noise > -0.3) tmpColor = aqua;
    else if (noise < -0.5 && noise > -0.8) tmpColor = blue;

    color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = color * fragment.intensity;

    return fragment;
}

void render() override{
    Model::render();
    Orbit orbit = Orbit(getUniforms(), getRadius(), Color(0.0f,  255.0f, 0.0f));
    orbit.render();
  }
};