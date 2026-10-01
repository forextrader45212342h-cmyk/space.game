#include "OrbitBlackHoleRenderer.h"

#include <cmath>

namespace OrbitBlackHole
{
    void Renderer::SetSettings(
        const Settings& value)
    {
        settings = value;

        if (settings.schwarzschildRadius <= 0)
            settings.schwarzschildRadius = 1;

        if (settings.diskOuterRadius <
            settings.diskInnerRadius)
        {
            settings.diskOuterRadius =
                settings.diskInnerRadius;
        }
    }

    const Settings&
    Renderer::GetSettings() const
    {
        return settings;
    }

    float Renderer::CalculateLensing(
        float distance) const
    {
        if (distance <=
            settings.schwarzschildRadius)
            return 1000000.0f;

        return settings.lensStrength *
            settings.schwarzschildRadius /
            distance;
    }

    bool Renderer::IsInsideHorizon(
        float distance) const
    {
        return distance <=
               settings.schwarzschildRadius;
    }
}
