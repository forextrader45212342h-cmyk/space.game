#include "OrbitCelestialGravity.h"
#include "OrbitMath.h"

#include <cmath>

namespace Orbit
{
    namespace
    {
        constexpr double G =
            6.67430e-11;

        constexpr double SOFTENING =
            1000.0;
    }

    Vector3 OrbitCelestialGravity::CalculateBodyGravity(
        const Vector3& position,
        const CelestialBody& body) const
    {
        Vector3 delta =
            OrbitMath::Sub(
                body.Position,
                position);

        double distanceSquared =
            OrbitMath::LengthSquared(delta);

        distanceSquared =
            std::max(
                distanceSquared,
                SOFTENING * SOFTENING);

        const double distance =
            std::sqrt(distanceSquared);

        const double acceleration =
            G *
            body.MassKg /
            distanceSquared;

        return OrbitMath::Multiply(
            OrbitMath::Normalize(delta),
            acceleration);
    }

    Vector3 OrbitCelestialGravity::CalculateAcceleration(
        const Vector3& position,
        const std::vector<CelestialBody>& bodies) const
    {
        Vector3 total{};

        for (const CelestialBody& body : bodies)
        {
            if (!body.Active ||
                body.MassKg <= 0.0)
                continue;

            total =
                OrbitMath::Add(
                    total,
                    CalculateBodyGravity(
                        position,
                        body));
        }

        return total;
    }
}
