#pragma once
#include <SDL2/SDL.h>
#include "loadOBJ.h"
#include "setupVertexArray.h"
#include "uniforms.h"
#include "glm/ext.hpp"
#include "camera.h"
#include "shaders.h"
#include "triangle.h"
#include "primitiveAssembly.h"
#include <cmath>

class Model
{
public:
  Model(const char *path, Camera &camera, float scaleFactor = 1.0f, float radius = 0.0f, bool lightSource = false) :
   modelPath(path), camera(camera), radius(radius),lightSource(lightSource)
  {

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    if (!loadOBJ("../models/sphere.obj", vertices, normals, faces))
    {
      return;
    }

    modelVertex = setupVertexArray(vertices, faces, normals);

    float a = 45.0f;
    rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis

    translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));

    // Projection matrix
    float fovInDegrees = 45.0f;
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    float nearClip = 0.1f;
    float farClip = 100.0f;
    uniforms.projection = glm::perspective(glm::radians(fovInDegrees), aspectRatio, nearClip, farClip);

    uniforms.viewport = createViewportMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);
  }

public:
  void rotateY(const float degree = 2.0f)
  {
    yRotation += degree;
  }

public:
  void render()
  {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(yRotation), rotationAxis);
    // Calculate the model matrix
    uniforms.model = translation * rotation * scale;

    uniforms.view = glm::lookAt(
        camera.cameraPosition, // The position of the camera
        camera.targetPosition, // The point the camera is looking at
        camera.upVector        // The up vector defining the camera's orientation
    );

    renderPipeline();
  }

public:
  void translate(const float degree = 3.00f)
  {
    if (radius == 0.0f)
      return;

    // Incrementa el ángulo de traslación
    translationAngle += glm::radians(degree);

    // Calcula la nueva posición en coordenadas polares
    float x = initialPosition.x + radius * std::sin(translationAngle);
    float z = initialPosition.z + radius * std::cos(translationAngle);

    // Actualiza la matriz de traslación
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, initialPosition.y, z));
  }

private:
  void renderPipeline()
  {

    // vertex shader
    std::vector<Vertex> transformedVertexes;
    for (int i = 0; i < modelVertex.size(); i += 2)
    {
      Vertex vertex = {modelVertex[i], modelVertex[i + 1]};
      Vertex transformedVertex = vertexShader(vertex, uniforms);
      transformedVertexes.push_back(transformedVertex);
    }

    // primitive assembly
    std::vector<std::vector<Vertex>> triangles = primitiveAssembly(transformedVertexes);

    // rasterize
    std::vector<Fragment> fragments;
    for (std::vector<Vertex> tr : triangles)
    {
      std::vector<Fragment> rasterizedTriangle = triangle(tr[0], tr[1], tr[2], calculateLightDirection(), lightSource);

      fragments.insert(fragments.end(), rasterizedTriangle.begin(), rasterizedTriangle.end());
    }

    // fragment shader
    for (Fragment fragment : fragments)
    {

      const Fragment &shadedFragment = fragmentShader(fragment);
      point(shadedFragment);
    }
  }

  

  glm::vec3 calculateLightDirection()
  {
    return -glm::normalize(getCurrentPosition());
  }

  glm::mat4 createViewportMatrix(size_t screenWidth, size_t screenHeight)
  {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
  }

public:
  glm::vec3 getCurrentPosition()
  {
    glm::vec4 direction = translation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(direction);
  }

  void setPosition(glm::vec3 pos)
  {
    initialPosition = pos;
  }


  virtual Fragment fragmentShader(Fragment &fragment) = 0;

private:
  const char *modelPath;
  float yRotation = 45.0f;
  std::vector<glm::vec3> modelVertex;
  Uniforms uniforms;
  Camera &camera;
  glm::vec3 rotationAxis;
  glm::mat4 translation = glm::mat4(1.0f);
  glm::mat4 scale;
  const float radius;
  float translationAngle = 0.f;
  glm::vec3 initialPosition = glm::vec3(0.0f);
  bool lightSource;
};