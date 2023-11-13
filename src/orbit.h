#pragma once
#include "uniforms.h"
#include "glm/ext.hpp"
#include "camera.h"
#include "fragment.h"
#include <vector>
#include "shaders.h"
#include <cmath>
#include "framebuffer.h"

class Orbit
{
public:
  Orbit(Uniforms uniforms, float radius, Color color) : radius(radius), color(color)
  {

    this->uniforms = {
        glm::mat4(1.0f),
        uniforms.view,
        uniforms.projection,
        uniforms.viewport};
  }

public:
  void render()
  {
    // Agregar puntos para 360 grados
    std::vector<Vertex> transformedVertexes;
    for (int i = 0; i < 360; i += 2)
    {

      float grade = static_cast<float>(i) * 3.1416f / 180.0f;

      Vertex vertex = {glm::vec3(radius * std::cos(grade), 0.0f, radius * std::sin(grade)), glm::vec3(1.0f)};
      Vertex transformedVertex = vertexShader(vertex, uniforms);
      transformedVertexes.push_back(transformedVertex);
    }

    for (Vertex vert : transformedVertexes)
    {

      if (vert.position.x < 0 || vert.position.y < 0  ||  vert.position.y > WINDOW_HEIGHT || vert.position.x > WINDOW_WIDTH) 
        continue;

      Fragment fragment = {
          static_cast<uint16_t>(vert.position.x),
          static_cast<uint16_t>(vert.position.y),
          static_cast<double>(vert.position.z),
          color};
      point(fragment);
    }
  }

private:
  Uniforms uniforms;
  float radius;
  Color color;
};