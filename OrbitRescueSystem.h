#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    enum class RescueStatus
    {
        Waiting,
        Searching,
        Located,
        Evacuating,
        Rescued,
        Failed
    };

    struct LostPerson
    {
        std::uint64_t Id = 0;

        std::string Name;

        Vector3 Position{};

        double Health = 100.0;

        RescueStatus Status =
            RescueStatus::Waiting;
    };

    class OrbitRescueSystem
    {
    public:
        void AddLostPerson(
            const LostPerson& person);

        void ReportPosition(
            std::uint64_t personId,
            const Vector3& position);

        void StartSearch(
            std::uint64_t personId);

        void Update(
            double deltaSeconds,
            const Vector3& rescueTeamPosition);

        LostPerson* FindPerson(
            std::uint64_t id);

        const std::vector<LostPerson>&
            GetPeople() const;

    private:
        std::vector<LostPerson> People;
    };
}
