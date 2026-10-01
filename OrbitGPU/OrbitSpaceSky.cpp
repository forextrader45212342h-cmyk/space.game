#include "OrbitSpaceSky.h"

#include <cmath>
#include <cstdint>

namespace OrbitSpace
{
    void Sky::Generate(
        uint32_t count,
        uint32_t seed)
    {
        stars.clear();
        stars.reserve(count);

        uint32_t state = seed;

        auto random01 =
            [&state]()
        {
            state =
                state * 1664525u +
                1013904223u;

            return
                static_cast<float>(state) /
                static_cast<float>(
                    UINT32_MAX);
        };

        for (uint32_t i = 0;
             i < count;
             ++i)
        {
            const float z =
                random01() * 2.0f - 1.0f;

            const float angle =
                random01() *
                6.28318530718f;

            const float radial =
                std::sqrt(
                    1.0f - z * z);

            Star star;

            star.direction =
            {
                radial * std::cos(angle),
                z,
                radial * std::sin(angle)
            };

            star.brightness =
                0.2f +
                random01() * 4.0f;

            star.temperature =
                2500.0f +
                random01() * 8000.0f;

            star.size =
                0.5f +
                random01() * 2.0f;

            stars.push_back(star);
        }
    }

    const std::vector<Star>&
    Sky::GetStars() const
    {
        return stars;
    }

    size_t Sky::Count() const
    {
        return stars.size();
    }

    void Sky::Clear()
    {
        stars.clear();
    }
}
