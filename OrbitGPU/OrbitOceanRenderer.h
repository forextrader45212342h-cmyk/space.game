#pragma once

#include "OrbitPBR.h"

namespace OrbitOcean
{
    struct Settings
    {
        float radius = 6371000.0f;
        float waveHeight = 2.0f;
        float waveLength = 100.0f;
        float waveSpeed = 1.0f;

        OrbitPBR::Color waterColor
        {
            0.02f,
            0.18f,
            0.25f,
            1.0f
        };

        float roughness = 0.08f;
    };

    class Renderer
    {
    private:
        Settings settings{};

    public:
        void SetSettings(
            const Settings& value);

        const Settings& GetSettings() const;

        float WaveHeight(
            float x,
            float z,
            float time) const;
    };
}
