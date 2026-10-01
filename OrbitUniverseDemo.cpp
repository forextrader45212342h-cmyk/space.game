#include "OrbitUniverseManager.h"
#include <iostream>

int main()
{
    Orbit::OrbitUniverseManager universe;

    universe.Initialize(
        123456789ULL,
        100);

    std::cout
        << "PROJECT ORBIT UNIVERSE\n";

    std::cout
        << "Star systems: "
        << universe.GetSystems().size()
        << "\n";

    for (const auto& system :
         universe.GetSystems())
    {
        std::cout
            << system.GetId()
            << " | "
            << system.GetName()
            << " | Stars: "
            << system.GetStars().size()
            << " | Planets: "
            << system.GetPlanets().size()
            << " | Black holes: "
            << system.GetBlackHoles().size()
            << "\n";
    }

    const Orbit::Vector3 start{
        0.0,
        0.0,
        0.0
    };

    const Orbit::Vector3 destination{
        6.3e16,
        0.0,
        0.0
    };

    const auto travel =
        universe.CalculateTravel(
            start,
            destination,
            0.20);

    std::cout
        << "\nINTERSTELLAR TRAVEL\n";

    std::cout
        << "Distance: "
        << travel.DistanceMeters
        << " m\n";

    std::cout
        << "Speed: "
        << travel.SpeedMetersPerSecond
        << " m/s\n";

    std::cout
        << "Travel days: "
        << travel.TravelDays
        << "\n";

    std::cout
        << "Travel years: "
        << travel.TravelYears
        << "\n";

    return 0;
}
