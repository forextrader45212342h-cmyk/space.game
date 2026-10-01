#include "OrbitWorldStreaming.h"

#include <cmath>

namespace Orbit
{
    std::uint64_t OrbitWorldStreaming::MakeKey(
        std::int64_t x,
        std::int64_t y,
        std::int64_t z) const
    {
        std::uint64_t a =
            static_cast<std::uint64_t>(
                x) * 73856093ULL;

        std::uint64_t b =
            static_cast<std::uint64_t>(
                y) * 19349663ULL;

        std::uint64_t c =
            static_cast<std::uint64_t>(
                z) * 83492791ULL;

        return a ^ b ^ c;
    }

    void OrbitWorldStreaming::Initialize(
        double cellSizeMeters,
        double loadDistanceMeters,
        double unloadDistanceMeters)
    {
        CellSize = cellSizeMeters;
        LoadDistance = loadDistanceMeters;
        UnloadDistance = unloadDistanceMeters;
        Cells.clear();
    }

    void OrbitWorldStreaming::Update(
        const Vector3& playerPosition)
    {
        const auto cellX =
            static_cast<std::int64_t>(
                std::floor(
                    playerPosition.X / CellSize));

        const auto cellY =
            static_cast<std::int64_t>(
                std::floor(
                    playerPosition.Y / CellSize));

        const auto cellZ =
            static_cast<std::int64_t>(
                std::floor(
                    playerPosition.Z / CellSize));

        const int radius =
            static_cast<int>(
                std::ceil(
                    LoadDistance / CellSize));

        for (int z = -radius; z <= radius; ++z)
        {
            for (int y = -radius; y <= radius; ++y)
            {
                for (int x = -radius; x <= radius; ++x)
                {
                    const auto cx = cellX + x;
                    const auto cy = cellY + y;
                    const auto cz = cellZ + z;

                    const std::uint64_t key =
                        MakeKey(cx, cy, cz);

                    WorldCell& cell =
                        Cells[key];

                    cell.X = cx;
                    cell.Y = cy;
                    cell.Z = cz;

                    const double centerX =
                        (cx + 0.5) * CellSize;

                    const double centerY =
                        (cy + 0.5) * CellSize;

                    const double centerZ =
                        (cz + 0.5) * CellSize;

                    const double dx =
                        centerX - playerPosition.X;

                    const double dy =
                        centerY - playerPosition.Y;

                    const double dz =
                        centerZ - playerPosition.Z;

                    cell.Distance =
                        std::sqrt(
                            dx * dx +
                            dy * dy +
                            dz * dz);

                    cell.Loaded =
                        cell.Distance <=
                        LoadDistance;
                }
            }
        }

        for (auto it = Cells.begin();
             it != Cells.end();)
        {
            if (it->second.Distance >
                UnloadDistance)
            {
                it = Cells.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    bool OrbitWorldStreaming::IsLoaded(
        std::int64_t x,
        std::int64_t y,
        std::int64_t z) const
    {
        const auto key =
            MakeKey(x, y, z);

        auto it = Cells.find(key);

        return
            it != Cells.end() &&
            it->second.Loaded;
    }

    const std::unordered_map<
        std::uint64_t,
        WorldCell>&
    OrbitWorldStreaming::GetCells() const
    {
        return Cells;
    }
}
