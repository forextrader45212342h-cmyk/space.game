#pragma once

#include "OrbitCelestialTypes.h"
#include <vector>

namespace Orbit
{
    class OrbitCelestialGravity
    {
    public:
        Vector3 CalculateAcceleration(
            const Vector3& position,
            const std::vector<CelestialBody>& bodies) const;

        Vector3 CalculateBodyGravity(
            const Vector3& position,
            const CelestialBody& body) const;
    };
}
