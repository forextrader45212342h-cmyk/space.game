#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    enum class AircraftMission
    {
        None,
        Passenger,
        Cargo,
        Rescue,
        Airdrop
    };

    struct AirdropPackage
    {
        std::uint64_t Id = 0;

        std::string Name;

        double MassKg = 10.0;

        Vector3 Target{};

        bool Released = false;
        bool Delivered = false;
    };

    struct Aircraft
    {
        std::uint64_t Id = 0;

        std::string Name;

        Vector3 Position{};
        Vector3 Velocity{};

        double Speed = 0.0;
        double CruiseSpeed = 250.0;

        AircraftMission Mission =
            AircraftMission::None;

        std::vector<AirdropPackage> Cargo;

        bool Active = true;
    };

    class OrbitAircraftSystem
    {
    public:
        void AddAircraft(
            const Aircraft& aircraft);

        void Update(
            double deltaSeconds);

        bool AssignAirdrop(
            std::uint64_t aircraftId,
            const AirdropPackage& package);

        bool ReleaseAirdrop(
            std::uint64_t aircraftId,
            std::uint64_t packageId);

        Aircraft* FindAircraft(
            std::uint64_t id);

        const std::vector<Aircraft>&
            GetAircraft() const;

    private:
        std::vector<Aircraft> AircraftList;
    };
}
