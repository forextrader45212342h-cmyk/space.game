#pragma once

#include "OrbitPBR.h"

#include <vector>
#include <cstdint>

namespace OrbitSpace
{
    struct Star
    {
        OrbitPBR::Vec3 direction{};

        float brightness = 1.0f;
        float temperature = 5778.0f;
        float size = 1.0f;
    };

    class Sky
    {
    private:
        std::vector<Star> stars;

    public:
        void Generate(
            uint32_t count,
            uint32_t seed = 1337);

        const std::vector<Star>&
        GetStars() const;

        size_t Count() const;

        void Clear();
    };
}
