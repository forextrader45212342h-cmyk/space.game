#pragma once

#include "OrbitTypes.h"
#include <cstdint>
#include <string>

namespace Orbit
{
    enum class CelestialBodyType
    {
        Star,
        Planet,
        Moon,
        BlackHole,
        Asteroid
    };

    struct CelestialBody
    {
        std::uint64_t Id = 0;
        std::string Name;

        CelestialBodyType Type = CelestialBodyType::Planet;

        double MassKg = 0.0;
        double RadiusMeters = 0.0;

        Vector3 Position{};
        Vector3 Velocity{};

        bool Active = true;
    };

    struct StarData
    {
        CelestialBody Body;

        double LuminositySolar = 1.0;
        double TemperatureKelvin = 5778.0;
        double AgeYears = 4.6e9;

        std::uint32_t PlanetCount = 0;
    };

    struct ExoplanetData
    {
        CelestialBody Body;

        double SemiMajorAxisMeters = 0.0;
        double OrbitalPeriodSeconds = 0.0;

        double SurfaceTemperatureKelvin = 288.0;
        double AtmosphericPressurePa = 101325.0;

        double WaterFraction = 0.0;
        double OxygenFraction = 0.21;

        bool HasAtmosphere = true;
        bool Habitable = false;

        double LifeProbability = 0.0;
    };

    struct BlackHoleData
    {
        CelestialBody Body;

        double SchwarzschildRadiusMeters = 0.0;
        double Spin = 0.0;

        double AccretionDiskInnerRadiusMeters = 0.0;
        double AccretionDiskOuterRadiusMeters = 0.0;
    };
}
