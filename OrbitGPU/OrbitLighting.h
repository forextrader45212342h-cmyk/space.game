#pragma once

#include "OrbitPBR.h"

#include <vector>
#include <cstdint>

namespace OrbitLighting
{
    enum class Type
    {
        Directional,
        Point,
        Spot
    };

    struct Light
    {
        Type type = Type::Directional;

        OrbitPBR::Vec3 position{};
        OrbitPBR::Vec3 direction{0,-1,0};

        OrbitPBR::Color color{1,1,1,1};

        float intensity = 1.0f;
        float range = 100.0f;

        float innerCone = 0.5f;
        float outerCone = 0.8f;

        bool castsShadow = true;
    };

    class LightSystem
    {
    private:
        std::vector<Light> lights;

    public:
        uint32_t Add(const Light& light);
        bool Remove(uint32_t index);

        void Clear();

        const std::vector<Light>& GetLights() const;

        size_t Count() const;
    };
}
