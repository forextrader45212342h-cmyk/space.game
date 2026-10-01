#pragma once

#include "OrbitStar.h"
#include "OrbitExoplanet.h"
#include "OrbitBlackHole.h"
#include "OrbitAsteroidField.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    class OrbitStarSystem
    {
    public:
        OrbitStarSystem() = default;

        explicit OrbitStarSystem(
            std::uint64_t id,
            const std::string& name);

        std::uint64_t GetId() const;
        const std::string& GetName() const;

        void AddStar(const OrbitStar& star);
        void AddPlanet(const OrbitExoplanet& planet);
        void AddBlackHole(const OrbitBlackHole& blackHole);
        void AddAsteroidField(
            const OrbitAsteroidField& field);

        std::vector<OrbitStar>& GetStars();
        std::vector<OrbitExoplanet>& GetPlanets();
        std::vector<OrbitBlackHole>& GetBlackHoles();
        std::vector<OrbitAsteroidField>& GetAsteroidFields();

        const std::vector<OrbitStar>& GetStars() const;
        const std::vector<OrbitExoplanet>& GetPlanets() const;
        const std::vector<OrbitBlackHole>& GetBlackHoles() const;
        const std::vector<OrbitAsteroidField>&
            GetAsteroidFields() const;

        void Update(double deltaSeconds);

    private:
        std::uint64_t Id = 0;
        std::string Name;

        std::vector<OrbitStar> Stars;
        std::vector<OrbitExoplanet> Planets;
        std::vector<OrbitBlackHole> BlackHoles;
        std::vector<OrbitAsteroidField> AsteroidFields;
    };
}
