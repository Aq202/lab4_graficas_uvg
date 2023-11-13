#pragma once
#include "model.h"

class PlanetRandom2Model : public Model
{
public:
  PlanetRandom2Model(Camera &camera) : Model("../models/sphere.obj", camera, 0.9f, 22.0f)
  {
  }

  Fragment fragmentShader(Fragment& fragment) {
    Color color = Color(59, 79, 188);

    // Coordenadas de textura UV
    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    for(int i = 1; i <= 3; i++){

    

    // Parámetros de ruido
    float offset_x = 2000 * i;
    float offset_y = 6000 * i;
    float zoom = 200.0;

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    // Genera el valor de ruido en función de las coordenadas de textura y el tiempo
    float noise = noiseGenerator.GetNoise((uv.x + offset_x) * zoom, (uv.y + offset_y) * zoom);

        if(i == 1 && noise > 0.5) color = Color(210, 239, 240);
        if(i == 2 && noise > 0.7) color = Color(216, 195, 145);
        if(i == 3 && (noise > 0.8) ) color = Color(194, 114, 6);

    }

    fragment.color = color * fragment.intensity;

    return fragment;
}

void render() override{
    Model::render();
    Orbit orbit = Orbit(getUniforms(), getRadius(), Color(0.0f,  255.0f, 0.0f));
    orbit.render();
  }
};