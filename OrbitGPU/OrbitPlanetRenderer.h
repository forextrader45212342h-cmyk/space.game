#pragma once

#include "OrbitPBR.h"

#include <vector>
#include <cstdint>

namespace OrbitPlanet
{
    struct Vertex
    {
        OrbitPBR::Vec3 position;
        OrbitPBR::Vec3 normal;

        float u = 0;
        float v = 0;
    };

    class Sphere
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float radius = 1.0f;

    public:
        bool Generate(
            float planetRadius,
            uint32_t subdivisions);

        const std::vector<Vertex>&
        GetVertices() const;

        const std::vector<uint32_t>&
        GetIndices() const;

        uint64_t TriangleCount() const;
    };
}
