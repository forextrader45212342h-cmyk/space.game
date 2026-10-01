#include "OrbitSeamlessTransition.h"

#include <algorithm>

namespace Orbit
{
    WorldLayer
    OrbitSeamlessTransition::DetermineLayer(
        double altitudeMeters,
        double planetRadiusMeters,
        double starDistanceMeters) const
    {
        if (altitudeMeters < 20000.0)
            return WorldLayer::PlanetSurface;

        if (altitudeMeters < 100000.0)
            return WorldLayer::Atmosphere;

        if (altitudeMeters < 2000000.0)
            return WorldLayer::LowOrbit;

        if (altitudeMeters < 100000000.0)
            return WorldLayer::HighOrbit;

        if (starDistanceMeters <
            0.01 * 149597870700.0)
            return WorldLayer::DeepSpace;

        return WorldLayer::Interstellar;
    }

    double
    OrbitSeamlessTransition::CalculateBlend(
        double value,
        double start,
        double end) const
    {
        if (end <= start)
            return 1.0;

        const double t =
            (value - start) /
            (end - start);

        return std::clamp(
            t,
            0.0,
            1.0);
    }
}
