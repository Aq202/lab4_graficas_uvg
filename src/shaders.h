#pragma once
#include "fragment.h"
#include "FastNoiseLite.h"
#include <SDL2/SDL.h>
#include "uniforms.h"

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
/*
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
*/

/*
Fragment fragmentShader(Fragment& fragment) {
    Color color;

    float noise = rand() % 100;

    if(noise < 10 ) color = Color(78,0,0);
    else if(noise < 30 ) color = Color(251,186,98);
    else if(noise < 40) color = Color(252,100,0);
    else if (noise < 50) color = Color(239, 144, 55);
    else color = Color(207,24,3);

    fragment.color = color * fragment.intensity;

    return fragment;
}

*/

/*
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

*/

/*
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
*/

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

