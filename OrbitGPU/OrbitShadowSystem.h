#pragma once

#include <cstdint>
#include <vector>

namespace OrbitShadow
{
    struct Cascade
    {
        float nearDistance = 0.1f;
        float farDistance = 100.0f;

        uint32_t resolution = 2048;

        bool valid = false;
    };

    class System
    {
    private:
        std::vector<Cascade> cascades;
        bool enabled = true;

    public:
        void ConfigureCascades(
            uint32_t count,
            float cameraNear,
            float cameraFar,
            uint32_t resolution);

        void SetEnabled(bool value);

        bool IsEnabled() const;

        const std::vector<Cascade>&
        GetCascades() const;
    };
}
