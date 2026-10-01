#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{

enum class PlanetType
{
    Star,
    Rocky,
    GasGiant,
    IceGiant,
    Dwarf
};

struct PlanetInfo
{
    std::string name;
    PlanetType type;

    double orbitalRadiusKm = 0.0;
    double radiusKm = 0.0;
    double gravity = 0.0;
    double rotationHours = 0.0;

    float atmosphere = 0.0f;
    float temperatureC = 0.0f;

    bool hasOcean = false;
    bool hasLife = false;
    bool explorable = false;
};

class SolarSystem
{
public:
    SolarSystem();

    const std::vector<PlanetInfo>& GetPlanets() const;

    const PlanetInfo* FindPlanet(
        const std::string& name
    ) const;

private:
    std::vector<PlanetInfo> planets;
};

}
