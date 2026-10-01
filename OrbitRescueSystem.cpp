#include "OrbitRescueSystem.h"
#include "OrbitMath.h"

namespace Orbit
{
    void OrbitRescueSystem::AddLostPerson(
        const LostPerson& person)
    {
        People.push_back(person);
    }

    LostPerson*
    OrbitRescueSystem::FindPerson(
        std::uint64_t id)
    {
        for (auto& person : People)
        {
            if (person.Id == id)
                return &person;
        }

        return nullptr;
    }

    void OrbitRescueSystem::ReportPosition(
        std::uint64_t personId,
        const Vector3& position)
    {
        LostPerson* person =
            FindPerson(personId);

        if (!person)
            return;

        person->Position = position;

        if (person->Status ==
            RescueStatus::Waiting)
        {
            person->Status =
                RescueStatus::Located;
        }
    }

    void OrbitRescueSystem::StartSearch(
        std::uint64_t personId)
    {
        LostPerson* person =
            FindPerson(personId);

        if (person)
        {
            person->Status =
                RescueStatus::Searching;
        }
    }

    void OrbitRescueSystem::Update(
        double deltaSeconds,
        const Vector3& rescueTeamPosition)
    {
        for (auto& person : People)
        {
            if (person.Status !=
                RescueStatus::Located)
                continue;

            const double distance =
                OrbitMath::Distance(
                    person.Position,
                    rescueTeamPosition);

            if (distance < 100.0)
            {
                person.Status =
                    RescueStatus::Evacuating;
            }
        }

        for (auto& person : People)
        {
            if (person.Status ==
                RescueStatus::Evacuating)
            {
                person.Health +=
                    deltaSeconds * 0.5;

                if (person.Health >= 100.0)
                {
                    person.Health = 100.0;

                    person.Status =
                        RescueStatus::Rescued;
                }
            }
        }
    }

    const std::vector<LostPerson>&
    OrbitRescueSystem::GetPeople() const
    {
        return People;
    }
}
