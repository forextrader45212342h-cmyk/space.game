#pragma once

#include "OrbitTypes.h"

namespace Orbit
{
    struct WeatherState
    {
        double Temperature = 288.0;
        double Pressure = 101325.0;
        double Humidity = 0.5;
        double WindSpeed = 0.0;

        double CloudDensity = 0.0;
        double RainIntensity = 0.0;
        double SnowIntensity = 0.0;

        Vector3 WindDirection{};
    };

    class OrbitWeatherSystem
    {
    public:
        void Initialize(std::uint64_t seed);

        void Update(
            double deltaSeconds,
            double altitudeMeters);

        const WeatherState& GetState() const;

    private:
        std::uint64_t Seed = 0;
        WeatherState State;
        double Time = 0.0;
    };
}
