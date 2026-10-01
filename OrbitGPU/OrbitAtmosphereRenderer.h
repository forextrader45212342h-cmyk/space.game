#pragma once

#include "OrbitPBR.h"

namespace OrbitAtmosphere
{
    struct Settings
    {
        float planetRadius = 6371000.0f;
        float atmosphereHeight = 100000.0f;

        OrbitPBR::Color rayleigh
        {
            0.25f,
            0.55f,
            1.0f,
            1.0f
        };

        OrbitPBR::Color mie
        {
            1.0f,
            1.0f,
            1.0f,
            1.0f
        };

        float rayleighStrength = 1.0f;
        float mieStrength = 0.05f;
        float density = 1.0f;
    };

    class Renderer
    {
    private:
        Settings settings{};

    public:
        void SetSettings(
            const Settings& value);

        const Settings& GetSettings() const;

        float DensityAtAltitude(
            float altitude) const;
    };
}
