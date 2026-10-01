#pragma once

#include "OrbitTypes.h"

#include <unordered_map>
#include <vector>
#include <mutex>

namespace Orbit
{
    struct GravitySource
    {
        EntityId Id = 0;

        Vector3 Position{};

        // Gravitational parameter μ = G * M.
        double GravitationalParameter = 0.0;

        double InfluenceRadius = 0.0;

        bool Enabled = true;
    };

    struct PhysicsBody
    {
        EntityId Id = 0;

        Vector3 Position{};
        Vector3 Velocity{};
        Vector3 Acceleration{};

        double Mass = 1.0;

        bool Dynamic = true;
    };

    class OrbitPhysicsSimulation
    {
    public:

        OrbitPhysicsSimulation() = default;
        ~OrbitPhysicsSimulation() = default;

        bool Initialize();

        void Shutdown();

        EntityId CreateBody(
            const Vector3& Position,
            const Vector3& Velocity,
            double Mass
        );

        bool RemoveBody(EntityId Id);

        EntityId CreateGravitySource(
            const Vector3& Position,
            double GravitationalParameter,
            double InfluenceRadius
        );

        bool RemoveGravitySource(EntityId Id);

        void Simulate(double DeltaSeconds);

        bool GetBody(
            EntityId Id,
            PhysicsBody& OutBody
        ) const;

    private:

        Vector3 CalculateGravity(
            const PhysicsBody& Body
        ) const;

        void IntegrateBody(
            PhysicsBody& Body,
            double DeltaSeconds
        );

    private:

        mutable std::mutex Mutex;

        std::unordered_map<EntityId, PhysicsBody> Bodies;

        std::unordered_map<EntityId, GravitySource> GravitySources;

        EntityId NextEntityId = 1;

        bool bInitialized = false;
    };
}
