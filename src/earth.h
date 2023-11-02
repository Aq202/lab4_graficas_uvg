#pragma once
#include "model.h"

class EarthModel : public Model
{
public:
  EarthModel(Camera &camera) : Model("../models/sphere.obj", camera, glm::vec3(0.0f, 0.0f, 0.0f), 3.0f)
  {
  }

  Fragment fragmentShader(Fragment& fragment) {
    Color color;

    glm::vec3 groundColor = glm::vec3(65.0f, 115.0f, 60.0f);
    glm::vec3 oceanColor = glm::vec3(15.0f, 51.0f, 221.0f);
    glm::vec3 cloudColor = glm::vec3(241.0f, 239.0f, 239.0f);

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    float offset_x = 6000.0f;
    float offset_y = 700.0f;
    float zoom = 200.0f;

    float groundNoise = noiseGenerator.GetNoise((uv.x + offset_x) * zoom, (uv.y + offset_y) * zoom);

    glm::vec3 tmpColor = (groundNoise < 0.3) ? oceanColor : groundColor;

    float offset_cloud_x = 8300.0f;
    float offset_cloud_y = 3000.0f;
    float zoom_cloud = 450.0f;

    float cloudNoise = noiseGenerator.GetNoise((uv.x + offset_cloud_x) * zoom_cloud, (uv.y + offset_cloud_y) * zoom_cloud);
    
    if (cloudNoise > 0.4f) {
        tmpColor = cloudColor;
    }


    color = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = color * fragment.intensity;

    return fragment;
}
};