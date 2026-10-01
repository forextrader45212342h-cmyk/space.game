#include "OrbitLighting.h"

namespace OrbitLighting
{
    uint32_t LightSystem::Add(const Light& light)
    {
        lights.push_back(light);

        return static_cast<uint32_t>(
            lights.size() - 1);
    }

    bool LightSystem::Remove(uint32_t index)
    {
        if (index >= lights.size())
            return false;

        lights.erase(
            lights.begin() + index);

        return true;
    }

    void LightSystem::Clear()
    {
        lights.clear();
    }

    const std::vector<Light>&
    LightSystem::GetLights() const
    {
        return lights;
    }

    size_t LightSystem::Count() const
    {
        return lights.size();
    }
}
