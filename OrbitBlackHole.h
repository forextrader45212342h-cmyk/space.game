#pragma once

#include "OrbitCelestialTypes.h"

namespace Orbit
{
    class OrbitBlackHole
    {
    public:
        OrbitBlackHole() = default;

        explicit OrbitBlackHole(
            const BlackHoleData& data);

        void SetData(
            const BlackHoleData& data);

        const BlackHoleData& GetData() const;

        double CalculateSchwarzschildRadius() const;

        Vector3 CalculateGravity(
            const Vector3& position) const;

        double CalculateTidalAcceleration(
            const Vector3& position,
            double objectSizeMeters) const;

        bool IsInsideEventHorizon(
            const Vector3& position) const;

    private:
        BlackHoleData Data;
    };
}
