#include "OrbitProceduralTerrain.h"

#include <cmath>
#include <cstdint>

namespace OrbitTerrain
{
    void Generator::SetSettings(
        const Settings& value)
    {
        settings = value;

        if (settings.octaves == 0)
            settings.octaves = 1;

        if (settings.lacunarity <= 0)
            settings.lacunarity = 2.0f;

        if (settings.gain <= 0)
            settings.gain = 0.5f;
    }

    const Settings&
    Generator::GetSettings() const
    {
        return settings;
    }

    float Generator::Noise(
        int x,
        int y,
        int z) const
    {
        uint32_t h =
            static_cast<uint32_t>(x) *
            374761393u;

        h +=
            static_cast<uint32_t>(y) *
            668265263u;

        h +=
            static_cast<uint32_t>(z) *
            2147483647u;

        h += settings.seed;

        h ^= h >> 13;
        h *= 1274126177u;
        h ^= h >> 16;

        return
            static_cast<float>(h) /
            static_cast<float>(
                UINT32_MAX);
    }

    float Generator::Fractal(
        float x,
        float y,
        float z) const
    {
        float value = 0;
        float amplitude = 1;
        float frequency = 1;
        float total = 0;

        for (uint32_t i = 0;
             i < settings.octaves;
             ++i)
        {
            const int ix =
                static_cast<int>(
                    std::floor(x * frequency));

            const int iy =
                static_cast<int>(
                    std::floor(y * frequency));

            const int iz =
                static_cast<int>(
                    std::floor(z * frequency));

            const float n =
                Noise(ix, iy, iz) * 2.0f - 1.0f;

            value += n * amplitude;
            total += amplitude;

            amplitude *= settings.gain;
            frequency *= settings.lacunarity;
        }

        if (total <= 0)
            return 0;

        return value / total;
    }

    float Generator::Height(
        float x,
        float y,
        float z) const
    {
        const float noise =
            Fractal(
                x * settings.frequency,
                y * settings.frequency,
                z * settings.frequency);

        return noise * settings.amplitude;
    }
}
