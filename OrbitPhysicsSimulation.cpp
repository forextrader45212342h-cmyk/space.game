#include "OrbitPhysicsSimulation.h"

#include <algorithm>
#include <limits>

namespace Orbit
{
    bool OrbitPhysicsSimulation::Initialize()
    {
        std::scoped_lock Lock(Mutex);

        Bodies.clear();
        GravitySources.clear();

        NextEntityId = 1;
        bInitialized = true;

        return true;
    }

    void OrbitPhysicsSimulation::Shutdown()
    {
        std::scoped_lock Lock(Mutex);

        Bodies.clear();
        GravitySources.clear();

        bInitialized = false;
    }

    EntityId OrbitPhysicsSimulation::CreateBody(
        const Vector3& Position,
        const Vector3& Velocity,
        double Mass)
    {
        std::scoped_lock Lock(Mutex);

        if (!bInitialized || Mass <= 0.0)
        {
            return 0;
        }

        const EntityId Id = NextEntityId++;

        PhysicsBody Body;

        Body.Id = Id;
        Body.Position = Position;
        Body.Velocity = Velocity;
        Body.Mass = Mass;
        Body.Dynamic = true;

        Bodies.emplace(Id, Body);

        return Id;
    }

    bool OrbitPhysicsSimulation::RemoveBody(EntityId Id)
    {
        std::scoped_lock Lock(Mutex);

        return Bodies.erase(Id) > 0;
    }

    EntityId OrbitPhysicsSimulation::CreateGravitySource(
        const Vector3& Position,
        double GravitationalParameter,
        double InfluenceRadius)
    {
        std::scoped_lock Lock(Mutex);

        if (!bInitialized ||
            GravitationalParameter <= 0.0 ||
            InfluenceRadius <= 0.0)
        {
            return 0;
        }

        const EntityId Id = NextEntityId++;

        GravitySource Source;

        Source.Id = Id;
        Source.Position = Position;
        Source.GravitationalParameter = GravitationalParameter;
        Source.InfluenceRadius = InfluenceRadius;

        GravitySources.emplace(Id, Source);

        return Id;
    }

    bool OrbitPhysicsSimulation::RemoveGravitySource(EntityId Id)
    {
        std::scoped_lock Lock(Mutex);

        return GravitySources.erase(Id) > 0;
    }

    Vector3 OrbitPhysicsSimulation::CalculateGravity(
        const PhysicsBody& Body) const
    {
        Vector3 TotalAcceleration{};

        for (const auto& [Id, Source] : GravitySources)
        {
            if (!Source.Enabled)
            {
                continue;
            }

            const Vector3 Direction =
                Source.Position - Body.Position;

            const double DistanceSquared =
                Direction.LengthSquared();

            if (DistanceSquared <= std::numeric_limits<double>::epsilon())
            {
                continue;
            }

            const double Distance = std::sqrt(DistanceSquared);

            if (Distance > Source.InfluenceRadius)
            {
                continue;
            }

            const double InverseDistance =
                1.0 / Distance;

            const double AccelerationMagnitude =
                Source.GravitationalParameter *
                InverseDistance *
                InverseDistance;

            TotalAcceleration +=
                Direction *
                (AccelerationMagnitude * InverseDistance);
        }

        return TotalAcceleration;
    }

    void OrbitPhysicsSimulation::IntegrateBody(
        PhysicsBody& Body,
        double DeltaSeconds)
    {
        /*
         * Semi-implicit Euler integration.
         *
         * Velocity is updated before position.
         * This is inexpensive and considerably more stable
         * for game-scale simulation than naive Euler integration.
         */

        Body.Acceleration = CalculateGravity(Body);

        Body.Velocity +=
            Body.Acceleration * DeltaSeconds;

        Body.Position +=
            Body.Velocity * DeltaSeconds;
    }

    void OrbitPhysicsSimulation::Simulate(double DeltaSeconds)
    {
        std::scoped_lock Lock(Mutex);

        if (!bInitialized || DeltaSeconds <= 0.0)
        {
            return;
        }

        /*
         * Clamp pathological frame times.
         * This prevents a paused/backgrounded application
         * from generating an enormous physics step.
         */

        DeltaSeconds =
            std::min(DeltaSeconds, 0.25);

        for (auto& [Id, Body] : Bodies)
        {
            if (!Body.Dynamic)
            {
                continue;
            }

            IntegrateBody(
                Body,
                DeltaSeconds
            );
        }
    }

    bool OrbitPhysicsSimulation::GetBody(
        EntityId Id,
        PhysicsBody& OutBody) const
    {
        std::scoped_lock Lock(Mutex);

        const auto It = Bodies.find(Id);

        if (It == Bodies.end())
        {
            return false;
        }

        OutBody = It->second;

        return true;
    }
}
