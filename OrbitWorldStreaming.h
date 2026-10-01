#pragma once

#include "OrbitTypes.h"
#include <cstdint>
#include <unordered_map>

namespace Orbit
{
    struct WorldCell
    {
        std::int64_t X = 0;
        std::int64_t Y = 0;
        std::int64_t Z = 0;

        bool Loaded = false;
        double Distance = 0.0;
    };

    class OrbitWorldStreaming
    {
    public:
        void Initialize(
            double cellSizeMeters,
            double loadDistanceMeters,
            double unloadDistanceMeters);

        void Update(
            const Vector3& playerPosition);

        bool IsLoaded(
            std::int64_t x,
            std::int64_t y,
            std::int64_t z) const;

        const std::unordered_map<
            std::uint64_t,
            WorldCell>&
            GetCells() const;

    private:
        double CellSize = 1000.0;
        double LoadDistance = 5000.0;
        double UnloadDistance = 7000.0;

        std::unordered_map<
            std::uint64_t,
            WorldCell> Cells;

        std::uint64_t MakeKey(
            std::int64_t x,
            std::int64_t y,
            std::int64_t z) const;
    };
}
