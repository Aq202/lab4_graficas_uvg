#pragma once
#include "model.h"

class MoonModel : public Model
{
public:
  MoonModel(Camera &camera) : Model("../models/sphere.obj", camera,glm::vec3(0.0f, 0.0f, 0.0f), 0.81f, 3.0f)
  {
  }

  Fragment fragmentShader(Fragment& fragment) {
    Color color = Color(217,217,217);

    // Coordenadas de textura UV
    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);
   

    // Parámetros de ruido
    float offset_x = 10000;
    float offset_y = 10000;
    float zoom = 500.0;

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    // Genera el valor de ruido en función de las coordenadas de textura y el tiempo
    float noise = noiseGenerator.GetNoise((uv.x + offset_x) * zoom, (uv.y + offset_y) * zoom);

    if(noise > 0.7) color = Color(114,114,114);
    else if (noise < -0.8) color = Color(177,177,177);

    fragment.color = color * fragment.intensity;

    return fragment;
}
};