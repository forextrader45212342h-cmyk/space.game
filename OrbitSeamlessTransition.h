#pragma once

namespace Orbit
{
    enum class WorldLayer
    {
        PlanetSurface,
        Atmosphere,
        LowOrbit,
        HighOrbit,
        DeepSpace,
        Interstellar
    };

    class OrbitSeamlessTransition
    {
    public:
        WorldLayer DetermineLayer(
            double altitudeMeters,
            double planetRadiusMeters,
            double starDistanceMeters) const;

        double CalculateBlend(
            double value,
            double start,
            double end) const;
    };
}
