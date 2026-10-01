#pragma once

#include <cstdint>
#include <cmath>

namespace Orbit
{
    using EntityId = std::uint64_t;

    struct Vector3
    {
        double X = 0.0;
        double Y = 0.0;
        double Z = 0.0;

        constexpr Vector3() = default;

        constexpr Vector3(double x, double y, double z)
            : X(x), Y(y), Z(z)
        {
        }

        Vector3 operator+(const Vector3& Other) const
        {
            return {X + Other.X, Y + Other.Y, Z + Other.Z};
        }

        Vector3 operator-(const Vector3& Other) const
        {
            return {X - Other.X, Y - Other.Y, Z - Other.Z};
        }

        Vector3 operator*(double Scalar) const
        {
            return {X * Scalar, Y * Scalar, Z * Scalar};
        }

        Vector3& operator+=(const Vector3& Other)
        {
            X += Other.X;
            Y += Other.Y;
            Z += Other.Z;
            return *this;
        }

        double LengthSquared() const
        {
            return X * X + Y * Y + Z * Z;
        }

        double Length() const
        {
            return std::sqrt(LengthSquared());
        }
    };

    struct Transform
    {
        Vector3 Position{};
        Vector3 Rotation{};
        Vector3 Scale{1.0, 1.0, 1.0};
    };

    struct EngineConfig
    {
        std::uint32_t TargetFPS = 60;
        bool VSync = true;
        bool EnableValidation = false;
        bool PreferVulkan = false;
        std::uint32_t MaxPhysicsSubSteps = 8;
    };
}
