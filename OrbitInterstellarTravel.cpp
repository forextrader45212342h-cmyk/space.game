#include "OrbitInterstellarTravel.h"
#include "OrbitMath.h"

namespace Orbit
{
    TravelResult OrbitInterstellarTravel::Calculate(
        const Vector3& start,
        const Vector3& destination,
        double fractionOfLightSpeed) const
    {
        TravelResult result;

        result.DistanceMeters =
            OrbitMath::Distance(
                start,
                destination);

        if (fractionOfLightSpeed <= 0.0)
            return result;

        result.SpeedMetersPerSecond =
            SpeedOfLight *
            fractionOfLightSpeed;

        result.TravelSeconds =
            result.DistanceMeters /
            result.SpeedMetersPerSecond;

        result.TravelDays =
            result.TravelSeconds /
            86400.0;

        result.TravelYears =
            result.TravelDays /
            365.25;

        return result;
    }

    double OrbitInterstellarTravel::ApplyTimeCompression(
        double realSeconds,
        double timeScale) const
    {
        if (realSeconds <= 0.0)
            return 0.0;

        if (timeScale <= 0.0)
            return 0.0;

        return realSeconds * timeScale;
    }
}
