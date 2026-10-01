#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    struct TrainStation
    {
        std::uint64_t Id = 0;
        std::string Name;
        Vector3 Position{};
    };

    struct Train
    {
        std::uint64_t Id = 0;
        std::string Name;

        Vector3 Position{};
        Vector3 Velocity{};

        std::vector<std::uint64_t> Route;

        std::size_t CurrentStation = 0;

        double Speed = 0.0;
        double MaximumSpeed = 80.0;

        bool Moving = false;
    };

    class OrbitTrainSystem
    {
    public:
        void AddStation(
            const TrainStation& station);

        void AddTrain(
            const Train& train);

        void StartTrain(
            std::uint64_t trainId);

        void StopTrain(
            std::uint64_t trainId);

        void Update(
            double deltaSeconds);

        Train* FindTrain(
            std::uint64_t trainId);

        const std::vector<Train>&
            GetTrains() const;

        const std::vector<TrainStation>&
            GetStations() const;

    private:
        std::vector<TrainStation> Stations;
        std::vector<Train> Trains;
    };
}
