#pragma once

#include "OrbitTypes.h"

namespace Orbit
{
    struct SolarLightState
    {
        Vector3 Direction{};
        double Intensity = 1.0;
        double Temperature = 5778.0;
        double DistanceFromStar = 0.0;
    };

    class OrbitSolarLighting
    {
    public:
        SolarLightState Calculate(
            const Vector3& starPosition,
            double luminositySolar,
            double starTemperature,
            const Vector3& objectPosition) const;
    };
}
