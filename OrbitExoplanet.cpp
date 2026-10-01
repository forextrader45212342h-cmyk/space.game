#include "OrbitExoplanet.h"
#include "OrbitMath.h"

#include <cmath>

namespace Orbit
{
    OrbitExoplanet::OrbitExoplanet(const ExoplanetData& data)
        : Data(data)
    {
    }

    void OrbitExoplanet::SetData(const ExoplanetData& data)
    {
        Data = data;
    }

    const ExoplanetData& OrbitExoplanet::GetData() const
    {
        return Data;
    }

    void OrbitExoplanet::UpdateOrbit(
        double deltaSeconds,
        const Vector3& starPosition)
    {
        if (Data.OrbitalPeriodSeconds <= 0.0)
            return;

        constexpr double TWO_PI = 6.28318530717958647692;

        OrbitAngle +=
            TWO_PI *
            deltaSeconds /
            Data.OrbitalPeriodSeconds;

        if (OrbitAngle > TWO_PI)
            OrbitAngle = std::fmod(OrbitAngle, TWO_PI);

        Data.Body.Position = {
            starPosition.X +
                Data.SemiMajorAxisMeters * std::cos(OrbitAngle),

            starPosition.Y,

            starPosition.Z +
                Data.SemiMajorAxisMeters * std::sin(OrbitAngle)
        };
    }

    bool OrbitExoplanet::IsHabitable() const
    {
        return Data.Habitable;
    }

    double OrbitExoplanet::GetLifeProbability() const
    {
        return Data.LifeProbability;
    }
}
