#pragma once

#include <cstdint>

namespace OrbitTerrain
{
    struct Settings
    {
        float amplitude = 1000.0f;
        float frequency = 0.002f;

        uint32_t octaves = 6;

        float lacunarity = 2.0f;
        float gain = 0.5f;

        uint32_t seed = 1337;
    };

    class Generator
    {
    private:
        Settings settings{};

        float Noise(
            int x,
            int y,
            int z) const;

        float Fractal(
            float x,
            float y,
            float z) const;

    public:
        void SetSettings(
            const Settings& value);

        const Settings& GetSettings() const;

        float Height(
            float x,
            float y,
            float z) const;
    };
}
