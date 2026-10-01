#pragma once

#include "OrbitCelestialTypes.h"

namespace Orbit
{
    class OrbitExoplanet
    {
    public:
        OrbitExoplanet() = default;

        explicit OrbitExoplanet(const ExoplanetData& data);

        void SetData(const ExoplanetData& data);

        const ExoplanetData& GetData() const;

        void UpdateOrbit(
            double deltaSeconds,
            const Vector3& starPosition);

        bool IsHabitable() const;

        double GetLifeProbability() const;

    private:
        ExoplanetData Data;
        double OrbitAngle = 0.0;
    };
}
