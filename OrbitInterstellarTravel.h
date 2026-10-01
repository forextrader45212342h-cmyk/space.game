#pragma once

#include "OrbitTypes.h"

namespace Orbit
{
    struct TravelResult
    {
        double DistanceMeters = 0.0;

        double SpeedMetersPerSecond = 0.0;

        double TravelSeconds = 0.0;

        double TravelDays = 0.0;
        double TravelYears = 0.0;
    };

    class OrbitInterstellarTravel
    {
    public:
        static constexpr double SpeedOfLight =
            299792458.0;

        TravelResult Calculate(
            const Vector3& start,
            const Vector3& destination,
            double fractionOfLightSpeed) const;

        double ApplyTimeCompression(
            double realSeconds,
            double timeScale) const;
    };
}
