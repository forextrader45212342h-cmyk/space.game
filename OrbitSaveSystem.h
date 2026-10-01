#pragma once

#include "OrbitTypes.h"

#include <string>

namespace Orbit
{
    struct SaveState
    {
        Vector3 PlayerPosition{};

        Vector3 PlayerVelocity{};

        std::uint64_t CurrentSystem = 0;

        double SimulationTime = 0.0;

        double Credits = 0.0;
    };

    class OrbitSaveSystem
    {
    public:
        bool Save(
            const std::string& path,
            const SaveState& state) const;

        bool Load(
            const std::string& path,
            SaveState& state) const;
    };
}
