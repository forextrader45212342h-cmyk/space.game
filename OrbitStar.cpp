#include "OrbitStar.h"
#include <cmath>

namespace Orbit
{
    OrbitStar::OrbitStar(const StarData& data)
        : Data(data)
    {
    }

    void OrbitStar::SetData(const StarData& data)
    {
        Data = data;
    }

    const StarData& OrbitStar::GetData() const
    {
        return Data;
    }

    double OrbitStar::GetLuminosity() const
    {
        return Data.LuminositySolar;
    }

    double OrbitStar::GetTemperature() const
    {
        return Data.TemperatureKelvin;
    }

    double OrbitStar::GetHabitableZoneInnerMeters() const
    {
        constexpr double AU = 149597870700.0;

        const double luminosity = Data.LuminositySolar;

        return 0.95 * std::sqrt(luminosity) * AU;
    }

    double OrbitStar::GetHabitableZoneOuterMeters() const
    {
        constexpr double AU = 149597870700.0;

        const double luminosity = Data.LuminositySolar;

        return 1.67 * std::sqrt(luminosity) * AU;
    }
}
