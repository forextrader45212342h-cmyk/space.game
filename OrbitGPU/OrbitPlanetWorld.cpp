#include "OrbitPlanetWorld.h"

namespace Orbit
{

PlanetWorld::PlanetWorld()
{
    BuildEarth();
    BuildMars();
}

void PlanetWorld::BuildEarth()
{
    locations =
    {
        {
            1001,
            "Earth",
            "New Horizon City",
            "City",
            25.20,
            55.27,
            8.0,
            true,
            true
        },

        {
            1002,
            "Earth",
            "Orbital Launch Complex",
            "Spaceport",
            28.61,
            77.20,
            120.0,
            true,
            true
        },

        {
            1003,
            "Earth",
            "Pacific Research Station",
            "Research",
            0.50,
            -150.0,
            25.0,
            true,
            true
        },

        {
            1004,
            "Earth",
            "Sahara Solar Field",
            "Industrial",
            24.00,
            10.00,
            500.0,
            false,
            true
        },

        {
            1005,
            "Earth",
            "Mountain Rescue Base",
            "Rescue",
            35.00,
            75.00,
            3200.0,
            true,
            true
        }
    };
}

void PlanetWorld::BuildMars()
{
    locations.push_back(
        {
            2001,
            "Mars",
            "Mars Base Alpha",
            "Colony",
            4.589,
            137.441,
            -2500.0,
            true,
            true
        }
    );

    locations.push_back(
        {
            2002,
            "Mars",
            "Olympus Research Station",
            "Research",
            18.65,
            226.2,
            21000.0,
            false,
            true
        }
    );

    locations.push_back(
        {
            2003,
            "Mars",
            "Valles Mining Zone",
            "Mining",
            -14.0,
            -59.0,
            -3000.0,
            true,
            true
        }
    );

    locations.push_back(
        {
            2004,
            "Mars",
            "Dust Valley Outpost",
            "Outpost",
            -5.5,
            45.0,
            1000.0,
            true,
            true
        }
    );
}

const std::vector<WorldLocation>&
PlanetWorld::GetLocations() const
{
    return locations;
}

std::vector<WorldLocation>
PlanetWorld::GetPlanetLocations(
    const std::string& planet) const
{
    std::vector<WorldLocation> result;

    for (const auto& location : locations)
    {
        if (location.planet == planet)
            result.push_back(location);
    }

    return result;
}

}
