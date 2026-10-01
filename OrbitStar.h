#pragma once

#include "OrbitCelestialTypes.h"

namespace Orbit
{
    class OrbitStar
    {
    public:
        OrbitStar() = default;

        explicit OrbitStar(const StarData& data);

        void SetData(const StarData& data);

        const StarData& GetData() const;

        double GetLuminosity() const;

        double GetTemperature() const;

        double GetHabitableZoneInnerMeters() const;

        double GetHabitableZoneOuterMeters() const;

    private:
        StarData Data;
    };
}
