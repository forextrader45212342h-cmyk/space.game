#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{

struct WorldLocation
{
    std::uint64_t id = 0;

    std::string planet;
    std::string name;
    std::string type;

    double latitude = 0.0;
    double longitude = 0.0;
    double altitudeMeters = 0.0;

    bool populated = false;
    bool landingAllowed = false;
};

class PlanetWorld
{
public:
    PlanetWorld();

    const std::vector<WorldLocation>&
    GetLocations() const;

    std::vector<WorldLocation>
    GetPlanetLocations(
        const std::string& planet
    ) const;

private:
    std::vector<WorldLocation> locations;

    void BuildEarth();
    void BuildMars();
};

}
