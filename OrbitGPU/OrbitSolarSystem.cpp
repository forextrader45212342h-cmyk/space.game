#include "OrbitSolarSystem.h"

namespace Orbit
{

SolarSystem::SolarSystem()
{
    planets =
    {
        {
            "Sun",
            PlanetType::Star,
            0.0,
            696340.0,
            274.0,
            609.12,
            1.0f,
            5505.0f,
            false,
            false,
            false
        },

        {
            "Mercury",
            PlanetType::Rocky,
            57910000.0,
            2439.7,
            0.38,
            1407.6,
            0.01f,
            167.0f,
            false,
            false,
            true
        },

        {
            "Venus",
            PlanetType::Rocky,
            108200000.0,
            6051.8,
            0.90,
            5832.5,
            0.98f,
            464.0f,
            false,
            false,
            true
        },

        {
            "Earth",
            PlanetType::Rocky,
            149600000.0,
            6371.0,
            1.00,
            23.934f,
            1.00f,
            15.0f,
            true,
            true,
            true
        },

        {
            "Mars",
            PlanetType::Rocky,
            227940000.0,
            3389.5,
            0.38,
            24.623f,
            0.02f,
            -63.0f,
            false,
            false,
            true
        },

        {
            "Jupiter",
            PlanetType::GasGiant,
            778500000.0,
            69911.0,
            2.53,
            9.925f,
            0.90f,
            -110.0f,
            false,
            false,
            false
        },

        {
            "Saturn",
            PlanetType::GasGiant,
            1434000000.0,
            58232.0,
            1.06,
            10.656f,
            0.90f,
            -140.0f,
            false,
            false,
            false
        },

        {
            "Uranus",
            PlanetType::IceGiant,
            2871000000.0,
            25362.0,
            0.89,
            17.24f,
            0.80f,
            -195.0f,
            false,
            false,
            false
        },

        {
            "Neptune",
            PlanetType::IceGiant,
            4495000000.0,
            24622.0,
            1.14,
            16.11f,
            0.80f,
            -200.0f,
            false,
            false,
            false
        }
    };
}

const std::vector<PlanetInfo>&
SolarSystem::GetPlanets() const
{
    return planets;
}

const PlanetInfo*
SolarSystem::FindPlanet(const std::string& name) const
{
    for (const PlanetInfo& planet : planets)
    {
        if (planet.name == name)
            return &planet;
    }

    return nullptr;
}

}
