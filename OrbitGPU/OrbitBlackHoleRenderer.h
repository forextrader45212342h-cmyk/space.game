#pragma once

#include "OrbitPBR.h"

namespace OrbitBlackHole
{
    struct Settings
    {
        float massSolar = 1.0f;

        float schwarzschildRadius = 1.0f;

        float diskInnerRadius = 3.0f;
        float diskOuterRadius = 20.0f;

        float diskTemperature = 1000000.0f;

        float lensStrength = 1.0f;
        float emissionStrength = 4.0f;

        OrbitPBR::Color diskColor
        {
            1.0f,
            0.25f,
            0.03f,
            1.0f
        };
    };

    class Renderer
    {
    private:
        Settings settings{};

    public:
        void SetSettings(
            const Settings& value);

        const Settings& GetSettings() const;

        float CalculateLensing(
            float distance) const;

        bool IsInsideHorizon(
            float distance) const;
    };
}
