#pragma once

#include "OrbitTypes.h"

#include <cstdint>

namespace Orbit
{
    struct NetworkEntityState
    {
        std::uint64_t EntityId = 0;

        Vector3 Position{};
        Vector3 Velocity{};

        double RotationX = 0.0;
        double RotationY = 0.0;
        double RotationZ = 0.0;

        std::uint32_t Tick = 0;
    };

    class OrbitNetworkState
    {
    public:
        void Apply(
            const NetworkEntityState& state);

        const NetworkEntityState&
            GetState() const;

    private:
        NetworkEntityState State;
    };
}
