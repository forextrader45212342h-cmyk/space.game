#include "OrbitUniverseGenerator.h"

#include <cmath>
#include <string>

namespace Orbit
{
    namespace
    {
        constexpr double SUN_MASS =
            1.98847e30;

        constexpr double EARTH_MASS =
            5.9722e24;

        constexpr double AU =
            149597870700.0;

        constexpr double DAY =
            86400.0;
    }

    std::uint64_t OrbitUniverseGenerator::NextRandom(
        std::uint64_t& state) const
    {
        state =
            state * 6364136223846793005ULL +
            1442695040888963407ULL;

        return state;
    }

    double OrbitUniverseGenerator::RandomRange(
        std::uint64_t& state,
        double min,
        double max) const
    {
        const std::uint64_t value =
            NextRandom(state);

        const double normalized =
            static_cast<double>(
                value % 1000000ULL) /
            1000000.0;

        return min +
            (max - min) *
            normalized;
    }

    OrbitStarSystem
    OrbitUniverseGenerator::GenerateSystem(
        std::uint64_t seed,
        std::uint64_t systemId) const
    {
        OrbitStarSystem system(
            systemId,
            "ORBIT-" +
            std::to_string(systemId));

        std::uint64_t rng = seed;

        StarData starData;

        starData.Body.Id = systemId * 1000ULL + 1;
        starData.Body.Name = "Primary Star";
        starData.Body.Type =
            CelestialBodyType::Star;

        starData.Body.MassKg =
            SUN_MASS *
            RandomRange(
                rng,
                0.5,
                2.0);

        starData.Body.RadiusMeters =
            696340000.0 *
            RandomRange(
                rng,
                0.6,
                1.8);

        starData.LuminositySolar =
            RandomRange(
                rng,
                0.1,
                4.0);

        starData.TemperatureKelvin =
            RandomRange(
                rng,
                3500.0,
                9000.0);

        starData.Body.Position = {
            0.0,
            0.0,
            0.0
        };

        system.AddStar(
            OrbitStar(starData));

        const int planetCount =
            static_cast<int>(
                RandomRange(
                    rng,
                    3.0,
                    12.0));

        for (int i = 0;
             i < planetCount;
             ++i)
        {
            const double orbitalAU =
                RandomRange(
                    rng,
                    0.25 + i * 0.20,
                    0.60 + i * 0.35);

            ExoplanetData planet;

            planet.Body.Id =
                systemId * 1000ULL +
                static_cast<std::uint64_t>(10 + i);

            planet.Body.Name =
                "EXOPLANET-" +
                std::to_string(i + 1);

            planet.Body.Type =
                CelestialBodyType::Planet;

            planet.Body.MassKg =
                EARTH_MASS *
                RandomRange(
                    rng,
                    0.05,
                    8.0);

            planet.Body.RadiusMeters =
                6371000.0 *
                RandomRange(
                    rng,
                    0.3,
                    2.5);

            planet.SemiMajorAxisMeters =
                orbitalAU * AU;

            const double mu =
                6.67430e-11 *
                starData.Body.MassKg;

            planet.OrbitalPeriodSeconds =
                6.28318530717958647692 *
                std::sqrt(
                    std::pow(
                        planet.SemiMajorAxisMeters,
                        3.0) /
                    mu);

            planet.SurfaceTemperatureKelvin =
                RandomRange(
                    rng,
                    180.0,
                    420.0);

            planet.AtmosphericPressurePa =
                RandomRange(
                    rng,
                    1000.0,
                    300000.0);

            planet.WaterFraction =
                RandomRange(
                    rng,
                    0.0,
                    0.90);

            planet.OxygenFraction =
                RandomRange(
                    rng,
                    0.0,
                    0.30);

            planet.HasAtmosphere =
                planet.AtmosphericPressurePa >
                500.0;

            const double hzInner =
                0.95 *
                std::sqrt(
                    starData.LuminositySolar) *
                AU;

            const double hzOuter =
                1.67 *
                std::sqrt(
                    starData.LuminositySolar) *
                AU;

            planet.Habitable =
                planet.SemiMajorAxisMeters >= hzInner &&
                planet.SemiMajorAxisMeters <= hzOuter;

            planet.LifeProbability =
                planet.Habitable
                ? RandomRange(
                    rng,
                    0.1,
                    0.95)
                : RandomRange(
                    rng,
                    0.0,
                    0.10);

            planet.Body.Position = {
                planet.SemiMajorAxisMeters,
                0.0,
                0.0
            };

            system.AddPlanet(
                OrbitExoplanet(planet));

            const LifeProfile life =
                LifeSystem.GenerateLife(
                    planet.Body.Id,
                    planet.LifeProbability,
                    planet.WaterFraction,
                    planet.SurfaceTemperatureKelvin,
                    planet.OxygenFraction,
                    rng);

            (void)life;
        }

        if (RandomRange(
                rng,
                0.0,
                1.0) < 0.03)
        {
            BlackHoleData blackHole;

            blackHole.Body.Id =
                systemId * 1000ULL + 900;

            blackHole.Body.Name =
                "STELLAR-BLACK-HOLE";

            blackHole.Body.Type =
                CelestialBodyType::BlackHole;

            blackHole.Body.MassKg =
                SUN_MASS *
                RandomRange(
                    rng,
                    3.0,
                    30.0);

            blackHole.Body.Position = {
                RandomRange(
                    rng,
                    -10.0 * AU,
                    10.0 * AU),
                0.0,
                RandomRange(
                    rng,
                    -10.0 * AU,
                    10.0 * AU)
            };

            blackHole.SchwarzschildRadiusMeters =
                2.0 *
                6.67430e-11 *
                blackHole.Body.MassKg /
                (299792458.0 *
                 299792458.0);

            blackHole.AccretionDiskInnerRadiusMeters =
                blackHole.SchwarzschildRadiusMeters * 3.0;

            blackHole.AccretionDiskOuterRadiusMeters =
                blackHole.SchwarzschildRadiusMeters * 100.0;

            system.AddBlackHole(
                OrbitBlackHole(blackHole));
        }

        OrbitAsteroidField asteroidField;

        asteroidField.Generate(
            seed ^ 0xA57E001ULL,
            5000,
            starData.Body.Position,
            starData.Body.MassKg,
            2.0 * AU,
            4.0 * AU);

        system.AddAsteroidField(
            asteroidField);

        return system;
    }
}
