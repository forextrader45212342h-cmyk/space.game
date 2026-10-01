#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    enum class LifeStage
    {
        None,
        Microbial,
        SimpleOrganism,
        ComplexOrganism,
        Animal,
        Intelligent,
        Civilization
    };

    struct LifeProfile
    {
        std::uint64_t PlanetId = 0;

        LifeStage Stage = LifeStage::None;

        double Biomass = 0.0;
        double Population = 0.0;

        double TechnologyLevel = 0.0;

        bool HasOceans = false;
        bool HasLandLife = false;
        bool Intelligent = false;

        std::string SpeciesName;
    };

    class OrbitLifeSystem
    {
    public:
        LifeProfile GenerateLife(
            std::uint64_t planetId,
            double lifeProbability,
            double waterFraction,
            double temperatureKelvin,
            double oxygenFraction,
            std::uint64_t seed) const;

        void Evolve(
            LifeProfile& life,
            double deltaYears) const;
    };
}
