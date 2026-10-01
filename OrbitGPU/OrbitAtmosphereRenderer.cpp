#include "OrbitAtmosphereRenderer.h"

#include <cmath>

namespace OrbitAtmosphere
{
    void Renderer::SetSettings(
        const Settings& value)
    {
        settings = value;

        if (settings.atmosphereHeight < 1)
            settings.atmosphereHeight = 1;

        if (settings.density < 0)
            settings.density = 0;
    }

    const Settings&
    Renderer::GetSettings() const
    {
        return settings;
    }

    float Renderer::DensityAtAltitude(
        float altitude) const
    {
        if (altitude < 0)
            altitude = 0;

        if (altitude >=
            settings.atmosphereHeight)
            return 0;

        const float normalized =
            altitude /
            settings.atmosphereHeight;

        return settings.density *
            std::exp(
                -normalized * 6.0f);
    }
}
