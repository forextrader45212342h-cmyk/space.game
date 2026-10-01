#pragma once

#include "OrbitTypes.h"
#include <cmath>
#include <algorithm>

namespace OrbitMath
{
    inline double Dot(const Orbit::Vector3& a, const Orbit::Vector3& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    inline Orbit::Vector3 Cross(const Orbit::Vector3& a, const Orbit::Vector3& b)
    {
        return {
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        };
    }

    inline double LengthSquared(const Orbit::Vector3& v)
    {
        return Dot(v, v);
    }

    inline double DistanceSquared(
        const Orbit::Vector3& a,
        const Orbit::Vector3& b)
    {
        return LengthSquared({
            a.X - b.X,
            a.Y - b.Y,
            a.Z - b.Z
        });
    }

    inline double Distance(
        const Orbit::Vector3& a,
        const Orbit::Vector3& b)
    {
        return std::sqrt(DistanceSquared(a, b));
    }

    inline Orbit::Vector3 Normalize(const Orbit::Vector3& v)
    {
        const double len = std::sqrt(LengthSquared(v));

        if (len <= 1e-12)
            return {0.0, 0.0, 0.0};

        return {
            v.X / len,
            v.Y / len,
            v.Z / len
        };
    }

    inline Orbit::Vector3 Add(
        const Orbit::Vector3& a,
        const Orbit::Vector3& b)
    {
        return {
            a.X + b.X,
            a.Y + b.Y,
            a.Z + b.Z
        };
    }

    inline Orbit::Vector3 Sub(
        const Orbit::Vector3& a,
        const Orbit::Vector3& b)
    {
        return {
            a.X - b.X,
            a.Y - b.Y,
            a.Z - b.Z
        };
    }

    inline Orbit::Vector3 Multiply(
        const Orbit::Vector3& a,
        double s)
    {
        return {
            a.X * s,
            a.Y * s,
            a.Z * s
        };
    }

    inline double Clamp(
        double value,
        double minValue,
        double maxValue)
    {
        return std::clamp(value, minValue, maxValue);
    }
}
