#include "OrbitOceanRenderer.h"

#include <cmath>

namespace OrbitOcean
{
    void Renderer::SetSettings(
        const Settings& value)
    {
        settings = value;

        if (settings.waveLength <= 0)
            settings.waveLength = 100;

        if (settings.waveSpeed < 0)
            settings.waveSpeed = 0;
    }

    const Settings&
    Renderer::GetSettings() const
    {
        return settings;
    }

    float Renderer::WaveHeight(
        float x,
        float z,
        float time) const
    {
        const float frequency =
            6.28318530718f /
            settings.waveLength;

        return settings.waveHeight *
            std::sin(
                (x + z) *
                frequency +
                time * settings.waveSpeed);
    }
}
