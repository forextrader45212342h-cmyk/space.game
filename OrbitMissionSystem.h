#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    enum class MissionStatus
    {
        Locked,
        Active,
        Completed,
        Failed
    };

    struct OrbitMission
    {
        std::uint64_t Id = 0;

        std::string Name;
        std::string Description;

        Vector3 Target{};

        double RequiredDistance = 100.0;

        MissionStatus Status =
            MissionStatus::Locked;

        double Reward = 0.0;
    };

    class OrbitMissionSystem
    {
    public:
        void AddMission(
            const OrbitMission& mission);

        void Update(
            const Vector3& playerPosition);

        const std::vector<OrbitMission>&
            GetMissions() const;

    private:
        std::vector<OrbitMission> Missions;
    };
}
