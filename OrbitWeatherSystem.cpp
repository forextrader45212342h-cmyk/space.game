#include "OrbitWeatherSystem.h"

#include <cmath>

namespace Orbit
{
    void OrbitWeatherSystem::Initialize(
        std::uint64_t seed)
    {
        Seed = seed;

        State = {};
        State.Temperature = 288.0;
        State.Pressure = 101325.0;
        State.Humidity = 0.5;
    }

    void OrbitWeatherSystem::Update(
        double deltaSeconds,
        double altitudeMeters)
    {
        Time += deltaSeconds;

        const double wave =
            std::sin(Time * 0.0002);

        State.WindSpeed =
            5.0 + wave * 3.0;

        State.WindDirection = {
            std::cos(Time * 0.0001),
            0.0,
            std::sin(Time * 0.0001)
        };

        State.Humidity =
            0.5 +
            0.3 *
            std::sin(Time * 0.00015);

        State.Humidity =
            std::max(
                0.0,
                std::min(
                    1.0,
                    State.Humidity));

        State.CloudDensity =
            State.Humidity *
            std::max(
                0.0,
                1.0 -
                altitudeMeters / 15000.0);

        State.RainIntensity =
            State.CloudDensity *
            State.Humidity;

        State.SnowIntensity =
            State.CloudDensity *
            (State.Temperature < 273.15
                ? 1.0
                : 0.0);

        State.Pressure =
            101325.0 *
            std::exp(
                -altitudeMeters / 8500.0);
    }

    const WeatherState&
    OrbitWeatherSystem::GetState() const
    {
        return State;
    }
}
