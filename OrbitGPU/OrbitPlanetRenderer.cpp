#include "OrbitPlanetRenderer.h"

#include <cmath>

namespace OrbitPlanet
{
    static constexpr float PI =
        3.14159265358979323846f;

    static OrbitPBR::Vec3 Normalize(
        const OrbitPBR::Vec3& v)
    {
        const float length =
            std::sqrt(
                v.x * v.x +
                v.y * v.y +
                v.z * v.z);

        if (length <= 0.000001f)
            return {0,1,0};

        return
        {
            v.x / length,
            v.y / length,
            v.z / length
        };
    }

    bool Sphere::Generate(
        float planetRadius,
        uint32_t subdivisions)
    {
        if (planetRadius <= 0 ||
            subdivisions < 2)
            return false;

        radius = planetRadius;

        vertices.clear();
        indices.clear();

        const uint32_t rings =
            subdivisions;

        const uint32_t segments =
            subdivisions * 2;

        for (uint32_t y = 0;
             y <= rings;
             ++y)
        {
            const float v =
                static_cast<float>(y) /
                static_cast<float>(rings);

            const float phi =
                v * PI;

            const float sinPhi =
                std::sin(phi);

            const float cosPhi =
                std::cos(phi);

            for (uint32_t x = 0;
                 x <= segments;
                 ++x)
            {
                const float u =
                    static_cast<float>(x) /
                    static_cast<float>(segments);

                const float theta =
                    u * PI * 2.0f;

                OrbitPBR::Vec3 normal
                {
                    sinPhi * std::cos(theta),
                    cosPhi,
                    sinPhi * std::sin(theta)
                };

                normal = Normalize(normal);

                Vertex vertex;

                vertex.normal = normal;

                vertex.position =
                {
                    normal.x * radius,
                    normal.y * radius,
                    normal.z * radius
                };

                vertex.u = u;
                vertex.v = v;

                vertices.push_back(vertex);
            }
        }

        for (uint32_t y = 0;
             y < rings;
             ++y)
        {
            for (uint32_t x = 0;
                 x < segments;
                 ++x)
            {
                const uint32_t row =
                    segments + 1;

                const uint32_t a =
                    y * row + x;

                const uint32_t b =
                    a + 1;

                const uint32_t c =
                    a + row;

                const uint32_t d =
                    c + 1;

                indices.push_back(a);
                indices.push_back(c);
                indices.push_back(b);

                indices.push_back(b);
                indices.push_back(c);
                indices.push_back(d);
            }
        }

        return !vertices.empty() &&
               !indices.empty();
    }

    const std::vector<Vertex>&
    Sphere::GetVertices() const
    {
        return vertices;
    }

    const std::vector<uint32_t>&
    Sphere::GetIndices() const
    {
        return indices;
    }

    uint64_t Sphere::TriangleCount() const
    {
        return indices.size() / 3;
    }
}
