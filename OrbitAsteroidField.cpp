#include "OrbitAsteroidField.h"

#include <cmath>
#include <random>

namespace Orbit
{
    namespace
    {
        constexpr double G = 6.67430e-11;
        constexpr double TWO_PI = 6.28318530717958647692;
    }

    void OrbitAsteroidField::Generate(
        std::uint64_t seed,
        std::size_t count,
        const Vector3& parentPosition,
        double parentMassKg,
        double innerRadiusMeters,
        double outerRadiusMeters)
    {
        Asteroids.clear();
        Asteroids.reserve(count);

        std::mt19937_64 rng(seed);

        std::uniform_real_distribution<double>
            radiusDistribution(
                innerRadiusMeters,
                outerRadiusMeters);

        std::uniform_real_distribution<double>
            angleDistribution(
                0.0,
                TWO_PI);

        std::uniform_real_distribution<double>
            sizeDistribution(
                2.0,
                500.0);

        for (std::size_t i = 0; i < count; ++i)
        {
            Asteroid asteroid;

            asteroid.Id =
                seed ^
                static_cast<std::uint64_t>(i + 1);

            asteroid.OrbitRadiusMeters =
                radiusDistribution(rng);

            asteroid.OrbitAngle =
                angleDistribution(rng);

            asteroid.RadiusMeters =
                sizeDistribution(rng);

            asteroid.MassKg =
                asteroid.RadiusMeters *
                asteroid.RadiusMeters *
                asteroid.RadiusMeters *
                3000.0;

            asteroid.AngularVelocity =
                std::sqrt(
                    G *
                    parentMassKg /
                    std::pow(
                        asteroid.OrbitRadiusMeters,
                        3.0));

            asteroid.Position = {
                parentPosition.X +
                    asteroid.OrbitRadiusMeters *
                    std::cos(asteroid.OrbitAngle),

                parentPosition.Y,

                parentPosition.Z +
                    asteroid.OrbitRadiusMeters *
                    std::sin(asteroid.OrbitAngle)
            };

            Asteroids.push_back(asteroid);
        }
    }

    void OrbitAsteroidField::Update(
        double deltaSeconds,
        const Vector3& parentPosition)
    {
        for (Asteroid& asteroid : Asteroids)
        {
            asteroid.OrbitAngle +=
                asteroid.AngularVelocity *
                deltaSeconds;

            asteroid.Position = {
                parentPosition.X +
                    asteroid.OrbitRadiusMeters *
                    std::cos(asteroid.OrbitAngle),

                parentPosition.Y,

                parentPosition.Z +
                    asteroid.OrbitRadiusMeters *
                    std::sin(asteroid.OrbitAngle)
            };

            const double speed =
                asteroid.OrbitRadiusMeters *
                asteroid.AngularVelocity;

            asteroid.Velocity = {
                -std::sin(asteroid.OrbitAngle) * speed,
                0.0,
                std::cos(asteroid.OrbitAngle) * speed
            };
        }
    }

    const std::vector<Asteroid>&
    OrbitAsteroidField::GetAsteroids() const
    {
        return Asteroids;
    }
}
