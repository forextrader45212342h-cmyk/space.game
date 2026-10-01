#include "OrbitShadowSystem.h"

namespace OrbitShadow
{
    void System::ConfigureCascades(
        uint32_t count,
        float cameraNear,
        float cameraFar,
        uint32_t resolution)
    {
        cascades.clear();

        if (count == 0)
            return;

        if (cameraNear <= 0)
            cameraNear = 0.1f;

        if (cameraFar <= cameraNear)
            cameraFar = cameraNear + 1.0f;

        const float range =
            cameraFar - cameraNear;

        for (uint32_t i = 0; i < count; ++i)
        {
            const float a =
                static_cast<float>(i) /
                static_cast<float>(count);

            const float b =
                static_cast<float>(i + 1) /
                static_cast<float>(count);

            Cascade cascade;

            cascade.nearDistance =
                cameraNear + range * a;

            cascade.farDistance =
                cameraNear + range * b;

            cascade.resolution = resolution;
            cascade.valid = true;

            cascades.push_back(cascade);
        }
    }

    void System::SetEnabled(bool value)
    {
        enabled = value;
    }

    bool System::IsEnabled() const
    {
        return enabled;
    }

    const std::vector<Cascade>&
    System::GetCascades() const
    {
        return cascades;
    }
}
