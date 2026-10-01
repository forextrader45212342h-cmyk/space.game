#include "OrbitMissionSystem.h"
#include "OrbitMath.h"

namespace Orbit
{
    void OrbitMissionSystem::AddMission(
        const OrbitMission& mission)
    {
        Missions.push_back(mission);
    }

    void OrbitMissionSystem::Update(
        const Vector3& playerPosition)
    {
        for (OrbitMission& mission : Missions)
        {
            if (mission.Status !=
                MissionStatus::Active)
                continue;

            const double distance =
                OrbitMath::Distance(
                    playerPosition,
                    mission.Target);

            if (distance <=
                mission.RequiredDistance)
            {
                mission.Status =
                    MissionStatus::Completed;
            }
        }
    }

    const std::vector<OrbitMission>&
    OrbitMissionSystem::GetMissions() const
    {
        return Missions;
    }
}
