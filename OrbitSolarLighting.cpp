#include "OrbitSolarLighting.h"
#include "OrbitMath.h"

namespace Orbit
{
    SolarLightState
    OrbitSolarLighting::Calculate(
        const Vector3& starPosition,
        double luminositySolar,
        double starTemperature,
        const Vector3& objectPosition) const
    {
        SolarLightState state;

        const Vector3 direction =
            OrbitMath::Sub(
                starPosition,
                objectPosition);

        state.DistanceFromStar =
            OrbitMath::Distance(
                starPosition,
                objectPosition);

        state.Direction =
            OrbitMath::Normalize(direction);

        constexpr double AU =
            149597870700.0;

        const double normalizedDistance =
            state.DistanceFromStar / AU;

        state.Intensity =
            luminositySolar /
            std::max(
                normalizedDistance *
                normalizedDistance,
                0.000001);

        state.Temperature =
            starTemperature;

        return state;
    }
}
