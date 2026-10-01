#include "OrbitLifeSystem.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace Orbit
{
    LifeProfile OrbitLifeSystem::GenerateLife(
        std::uint64_t planetId,
        double lifeProbability,
        double waterFraction,
        double temperatureKelvin,
        double oxygenFraction,
        std::uint64_t seed) const
    {
        LifeProfile result;
        result.PlanetId = planetId;

        std::mt19937_64 rng(seed);
        std::uniform_real_distribution<double> chance(0.0, 1.0);

        const bool temperatureSuitable =
            temperatureKelvin >= 240.0 &&
            temperatureKelvin <= 340.0;

        const bool waterSuitable =
            waterFraction > 0.05;

        const bool oxygenSuitable =
            oxygenFraction > 0.05;

        double probability = lifeProbability;

        if (!temperatureSuitable)
            probability *= 0.20;

        if (!waterSuitable)
            probability *= 0.10;

        if (!oxygenSuitable)
            probability *= 0.50;

        probability = std::clamp(probability, 0.0, 1.0);

        if (chance(rng) > probability)
            return result;

        result.Stage = LifeStage::Microbial;
        result.Biomass = 1.0;
        result.HasOceans = waterSuitable;

        if (chance(rng) < 0.65)
        {
            result.Stage = LifeStage::SimpleOrganism;
            result.Biomass = 10.0;
        }

        if (chance(rng) < 0.35)
        {
            result.Stage = LifeStage::ComplexOrganism;
            result.Biomass = 100.0;
        }

        if (chance(rng) < 0.25)
        {
            result.Stage = LifeStage::Animal;
            result.Biomass = 1000.0;
            result.HasLandLife = true;
        }

        if (chance(rng) < 0.08)
        {
            result.Stage = LifeStage::Intelligent;
            result.Biomass = 5000.0;
            result.Intelligent = true;
            result.TechnologyLevel = 1.0;
        }

        if (chance(rng) < 0.30 &&
            result.Intelligent)
        {
            result.Stage = LifeStage::Civilization;
            result.TechnologyLevel = 2.0;
        }

        result.Population =
            std::max(1.0, result.Biomass * 1000.0);

        result.SpeciesName =
            "ORBIT-SPECIES-" +
            std::to_string(seed % 1000000);

        return result;
    }

    void OrbitLifeSystem::Evolve(
        LifeProfile& life,
        double deltaYears) const
    {
        if (life.Stage == LifeStage::None)
            return;

        if (deltaYears <= 0.0)
            return;

        const double growth =
            std::min(deltaYears * 0.001, 0.25);

        life.Biomass *= 1.0 + growth;

        if (life.Population > 0.0)
            life.Population *= 1.0 + growth * 0.5;

        if (life.Stage == LifeStage::Intelligent ||
            life.Stage == LifeStage::Civilization)
        {
            life.TechnologyLevel +=
                deltaYears * 0.00001;

            life.TechnologyLevel =
                std::min(life.TechnologyLevel, 100.0);
        }
    }
}
