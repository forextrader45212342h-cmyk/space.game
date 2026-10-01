#include "OrbitTrainSystem.h"
#include "OrbitMath.h"

#include <algorithm>

namespace Orbit
{
    void OrbitTrainSystem::AddStation(
        const TrainStation& station)
    {
        Stations.push_back(station);
    }

    void OrbitTrainSystem::AddTrain(
        const Train& train)
    {
        Trains.push_back(train);
    }

    void OrbitTrainSystem::StartTrain(
        std::uint64_t trainId)
    {
        Train* train =
            FindTrain(trainId);

        if (train)
            train->Moving = true;
    }

    void OrbitTrainSystem::StopTrain(
        std::uint64_t trainId)
    {
        Train* train =
            FindTrain(trainId);

        if (train)
        {
            train->Moving = false;
            train->Speed = 0.0;
            train->Velocity = {};
        }
    }

    Train* OrbitTrainSystem::FindTrain(
        std::uint64_t trainId)
    {
        for (auto& train : Trains)
        {
            if (train.Id == trainId)
                return &train;
        }

        return nullptr;
    }

    void OrbitTrainSystem::Update(
        double deltaSeconds)
    {
        for (Train& train : Trains)
        {
            if (!train.Moving)
                continue;

            if (train.Route.empty())
                continue;

            if (train.CurrentStation >=
                train.Route.size())
            {
                train.CurrentStation = 0;
            }

            const std::uint64_t targetId =
                train.Route[
                    train.CurrentStation];

            auto stationIt =
                std::find_if(
                    Stations.begin(),
                    Stations.end(),
                    [targetId](
                        const TrainStation& s)
                    {
                        return s.Id == targetId;
                    });

            if (stationIt ==
                Stations.end())
                continue;

            const Vector3 target =
                stationIt->Position;

            const Vector3 direction =
                OrbitMath::Normalize(
                    OrbitMath::Sub(
                        target,
                        train.Position));

            const double distance =
                OrbitMath::Distance(
                    train.Position,
                    target);

            train.Speed =
                std::min(
                    train.MaximumSpeed,
                    train.Speed +
                    10.0 *
                    deltaSeconds);

            train.Velocity =
                OrbitMath::Multiply(
                    direction,
                    train.Speed);

            train.Position =
                OrbitMath::Add(
                    train.Position,
                    OrbitMath::Multiply(
                        train.Velocity,
                        deltaSeconds));

            if (distance < 20.0)
            {
                train.Position = target;
                train.Speed = 0.0;
                train.Velocity = {};

                train.CurrentStation =
                    (train.CurrentStation + 1) %
                    train.Route.size();
            }
        }
    }

    const std::vector<Train>&
    OrbitTrainSystem::GetTrains() const
    {
        return Trains;
    }

    const std::vector<TrainStation>&
    OrbitTrainSystem::GetStations() const
    {
        return Stations;
    }
}
