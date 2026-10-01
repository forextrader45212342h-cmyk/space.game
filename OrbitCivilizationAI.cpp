#include "OrbitCivilizationAI.h"

#include <algorithm>

namespace Orbit
{
    void OrbitCivilizationAI::Update(
        CivilizationState& civilization,
        double deltaYears)
    {
        if (deltaYears <= 0.0)
            return;

        UpdateEconomy(
            civilization,
            deltaYears);

        UpdateTechnology(
            civilization,
            deltaYears);

        civilization.Population *=
            1.0 +
            std::min(
                deltaYears * 0.005,
                0.05);

        civilization.Energy =
            std::max(
                0.0,
                civilization.Energy);

        if (civilization.Technology >= 10.0)
            civilization.Spacefaring = true;

        if (civilization.Technology >= 50.0)
            civilization.Interstellar = true;
    }

    void OrbitCivilizationAI::UpdateEconomy(
        CivilizationState& civilization,
        double deltaYears) const
    {
        civilization.Economy +=
            deltaYears *
            (0.1 + civilization.Technology * 0.01);
    }

    void OrbitCivilizationAI::UpdateTechnology(
        CivilizationState& civilization,
        double deltaYears) const
    {
        civilization.Technology +=
            deltaYears *
            0.01 *
            (1.0 + civilization.Economy * 0.001);

        civilization.Technology =
            std::min(
                civilization.Technology,
                100.0);

        civilization.Military =
            civilization.Technology * 0.5;

        civilization.Energy =
            civilization.Technology * 10.0;
    }
}
