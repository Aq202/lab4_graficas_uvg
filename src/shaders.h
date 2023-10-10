#pragma once
#include "fragment.h"
#include "FastNoiseLite.h"
#include <SDL2/SDL.h>

Vertex vertexShader(const Vertex vertex, const Uniforms& uniforms) {
     glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.position
    };
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
    //SDL_Log("%f", noiseValue);

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
