#include "OrbitUniverseManager.h"

namespace Orbit
{
    void OrbitUniverseManager::Initialize(
        std::uint64_t universeSeed,
        std::size_t systemCount)
    {
        UniverseSeed = universeSeed;

        Systems.clear();
        Systems.reserve(systemCount);

        for (std::size_t i = 0;
             i < systemCount;
             ++i)
        {
            const std::uint64_t systemId =
                static_cast<std::uint64_t>(i + 1);

            const std::uint64_t seed =
                UniverseSeed ^
                (systemId *
                 0x9E3779B97F4A7C15ULL);

            Systems.push_back(
                Generator.GenerateSystem(
                    seed,
                    systemId));
        }

        Time.Reset();
    }

    void OrbitUniverseManager::Update(
        double realDeltaSeconds)
    {
        Time.Tick(realDeltaSeconds);

        const double simulationDelta =
            realDeltaSeconds *
            Time.GetTimeScale();

        for (OrbitStarSystem& system :
             Systems)
        {
            system.Update(simulationDelta);
        }
    }

    OrbitStarSystem*
    OrbitUniverseManager::GetSystem(
        std::uint64_t systemId)
    {
        for (OrbitStarSystem& system :
             Systems)
        {
            if (system.GetId() == systemId)
                return &system;
        }

        return nullptr;
    }

    const std::vector<OrbitStarSystem>&
    OrbitUniverseManager::GetSystems() const
    {
        return Systems;
    }

    Vector3 OrbitUniverseManager::CalculateGravity(
        std::uint64_t systemId,
        const Vector3& position) const
    {
        const OrbitStarSystem* system = nullptr;

        for (const OrbitStarSystem& candidate :
             Systems)
        {
            if (candidate.GetId() == systemId)
            {
                system = &candidate;
                break;
            }
        }

        if (!system)
            return {};

        std::vector<CelestialBody> bodies;

        for (const OrbitStar& star :
             system->GetStars())
        {
            bodies.push_back(
                star.GetData().Body);
        }

        for (const OrbitExoplanet& planet :
             system->GetPlanets())
        {
            bodies.push_back(
                planet.GetData().Body);
        }

        for (const OrbitBlackHole& blackHole :
             system->GetBlackHoles())
        {
            bodies.push_back(
                blackHole.GetData().Body);
        }

        return Gravity.CalculateAcceleration(
            position,
            bodies);
    }

    TravelResult OrbitUniverseManager::CalculateTravel(
        const Vector3& start,
        const Vector3& destination,
        double fractionOfLightSpeed) const
    {
        return Travel.Calculate(
            start,
            destination,
            fractionOfLightSpeed);
    }

    OrbitTimeSystem&
    OrbitUniverseManager::GetTimeSystem()
    {
        return Time;
    }
}
