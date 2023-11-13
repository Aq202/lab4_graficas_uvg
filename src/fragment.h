#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include "color.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 original;
  bool renderize;
};

struct Fragment {
  uint16_t x;      
  uint16_t y;      
  double z;
  Color color; 
  float intensity;
  glm::vec3 original; 
};

struct FragColor {
  Color color;
  double z; 
};
