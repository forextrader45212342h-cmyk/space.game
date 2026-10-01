#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <vector>

namespace Orbit
{
    struct DebrisPiece
    {
        std::uint64_t Id = 0;

        Vector3 Position{};
        Vector3 Velocity{};

        double MassKg = 1.0;
        double Lifetime = 30.0;

        bool Active = true;
    };

    class OrbitDestructionSystem
    {
    public:
        void CreateExplosion(
            const Vector3& position,
            const Vector3& impulse,
            int pieces,
            double energy);

        void Update(double deltaSeconds);

        const std::vector<DebrisPiece>&
            GetDebris() const;

    private:
        std::vector<DebrisPiece> Debris;
        std::uint64_t NextId = 1;
    };
}
