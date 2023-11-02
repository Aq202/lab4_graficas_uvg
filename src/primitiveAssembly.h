#pragma once
#include <vector>
#include "fragment.h"


std::vector<std::vector<Vertex>> primitiveAssembly(
    const std::vector<Vertex> &transformedVertexes)
{
    std::vector<std::vector<Vertex>> groupedVertexes;

    for (int i = 0; i < transformedVertexes.size(); i += 3)
    {
        std::vector<Vertex> triangle;
        triangle.push_back(transformedVertexes[i]);
        triangle.push_back(transformedVertexes[i + 1]);
        triangle.push_back(transformedVertexes[i + 2]);

        groupedVertexes.push_back(triangle);
    }

    return groupedVertexes;
}