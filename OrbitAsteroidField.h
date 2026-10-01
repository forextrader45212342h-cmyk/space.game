#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <vector>

namespace Orbit
{
    struct Asteroid
    {
        std::uint64_t Id = 0;

        Vector3 Position{};
        Vector3 Velocity{};

        double RadiusMeters = 1.0;
        double MassKg = 1.0;

        double OrbitRadiusMeters = 0.0;
        double OrbitAngle = 0.0;
        double AngularVelocity = 0.0;
    };

    class OrbitAsteroidField
    {
    public:
        void Generate(
            std::uint64_t seed,
            std::size_t count,
            const Vector3& parentPosition,
            double parentMassKg,
            double innerRadiusMeters,
            double outerRadiusMeters);

        void Update(
            double deltaSeconds,
            const Vector3& parentPosition);

        const std::vector<Asteroid>& GetAsteroids() const;

    private:
        std::vector<Asteroid> Asteroids;
    };
}
