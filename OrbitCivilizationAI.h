#pragma once

#include "OrbitTypes.h"
#include <cstdint>
#include <string>

namespace Orbit
{
    struct CivilizationState
    {
        std::uint64_t Id = 0;

        std::string Name;

        double Population = 0.0;

        double Technology = 0.0;
        double Energy = 0.0;
        double Military = 0.0;
        double Economy = 0.0;

        Vector3 HomePosition{};

        bool Spacefaring = false;
        bool Interstellar = false;
    };

    class OrbitCivilizationAI
    {
    public:
        void Update(
            CivilizationState& civilization,
            double deltaYears);

    private:
        void UpdateEconomy(
            CivilizationState& civilization,
            double deltaYears) const;

        void UpdateTechnology(
            CivilizationState& civilization,
            double deltaYears) const;
    };
}
