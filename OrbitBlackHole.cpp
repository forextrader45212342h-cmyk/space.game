#include "OrbitBlackHole.h"
#include "OrbitMath.h"

#include <cmath>

namespace Orbit
{
    namespace
    {
        constexpr double G =
            6.67430e-11;

        constexpr double C =
            299792458.0;
    }

    OrbitBlackHole::OrbitBlackHole(
        const BlackHoleData& data)
        : Data(data)
    {
    }

    void OrbitBlackHole::SetData(
        const BlackHoleData& data)
    {
        Data = data;
    }

    const BlackHoleData&
    OrbitBlackHole::GetData() const
    {
        return Data;
    }

    double OrbitBlackHole::CalculateSchwarzschildRadius() const
    {
        return
            2.0 *
            G *
            Data.Body.MassKg /
            (C * C);
    }

    Vector3 OrbitBlackHole::CalculateGravity(
        const Vector3& position) const
    {
        Vector3 delta =
            OrbitMath::Sub(
                Data.Body.Position,
                position);

        double distance =
            OrbitMath::Distance(
                Data.Body.Position,
                position);

        const double radius =
            CalculateSchwarzschildRadius();

        distance =
            std::max(distance, radius * 1.001);

        const double acceleration =
            G *
            Data.Body.MassKg /
            (distance * distance);

        return OrbitMath::Multiply(
            OrbitMath::Normalize(delta),
            acceleration);
    }

    double OrbitBlackHole::CalculateTidalAcceleration(
        const Vector3& position,
        double objectSizeMeters) const
    {
        const double distance =
            std::max(
                OrbitMath::Distance(
                    Data.Body.Position,
                    position),
                CalculateSchwarzschildRadius() * 1.001);

        return
            2.0 *
            G *
            Data.Body.MassKg *
            objectSizeMeters /
            (distance * distance * distance);
    }

    bool OrbitBlackHole::IsInsideEventHorizon(
        const Vector3& position) const
    {
        return
            OrbitMath::Distance(
                Data.Body.Position,
                position)
            <= CalculateSchwarzschildRadius();
    }
}
