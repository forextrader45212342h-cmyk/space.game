#include "OrbitStarSystem.h"

namespace Orbit
{
    OrbitStarSystem::OrbitStarSystem(
        std::uint64_t id,
        const std::string& name)
        : Id(id),
          Name(name)
    {
    }

    std::uint64_t OrbitStarSystem::GetId() const
    {
        return Id;
    }

    const std::string&
    OrbitStarSystem::GetName() const
    {
        return Name;
    }

    void OrbitStarSystem::AddStar(
        const OrbitStar& star)
    {
        Stars.push_back(star);
    }

    void OrbitStarSystem::AddPlanet(
        const OrbitExoplanet& planet)
    {
        Planets.push_back(planet);
    }

    void OrbitStarSystem::AddBlackHole(
        const OrbitBlackHole& blackHole)
    {
        BlackHoles.push_back(blackHole);
    }

    void OrbitStarSystem::AddAsteroidField(
        const OrbitAsteroidField& field)
    {
        AsteroidFields.push_back(field);
    }

    std::vector<OrbitStar>&
    OrbitStarSystem::GetStars()
    {
        return Stars;
    }

    std::vector<OrbitExoplanet>&
    OrbitStarSystem::GetPlanets()
    {
        return Planets;
    }

    std::vector<OrbitBlackHole>&
    OrbitStarSystem::GetBlackHoles()
    {
        return BlackHoles;
    }

    std::vector<OrbitAsteroidField>&
    OrbitStarSystem::GetAsteroidFields()
    {
        return AsteroidFields;
    }

    const std::vector<OrbitStar>&
    OrbitStarSystem::GetStars() const
    {
        return Stars;
    }

    const std::vector<OrbitExoplanet>&
    OrbitStarSystem::GetPlanets() const
    {
        return Planets;
    }

    const std::vector<OrbitBlackHole>&
    OrbitStarSystem::GetBlackHoles() const
    {
        return BlackHoles;
    }

    const std::vector<OrbitAsteroidField>&
    OrbitStarSystem::GetAsteroidFields() const
    {
        return AsteroidFields;
    }

    void OrbitStarSystem::Update(
        double deltaSeconds)
    {
        if (Stars.empty())
            return;

        const Vector3 starPosition =
            Stars.front().GetData().Body.Position;

        for (OrbitExoplanet& planet : Planets)
        {
            planet.UpdateOrbit(
                deltaSeconds,
                starPosition);
        }

        for (OrbitAsteroidField& field :
             AsteroidFields)
        {
            field.Update(
                deltaSeconds,
                starPosition);
        }
    }
}
