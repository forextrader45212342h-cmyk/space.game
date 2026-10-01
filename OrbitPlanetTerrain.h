#pragma once

#include "OrbitTypes.h"
#include <cstdint>
#include <vector>

namespace Orbit
{
    struct TerrainVertex
    {
        Vector3 Position{};
        Vector3 Normal{};
        double Height = 0.0;
    };

    struct TerrainChunk
    {
        std::int32_t X = 0;
        std::int32_t Y = 0;
        std::int32_t Resolution = 0;

        double SizeMeters = 0.0;

        std::vector<TerrainVertex> Vertices;
        std::vector<std::uint32_t> Indices;

        bool Loaded = false;
    };

    class OrbitPlanetTerrain
    {
    public:
        void Generate(
            std::uint64_t seed,
            double planetRadiusMeters,
            double maxTerrainHeightMeters,
            int resolution);

        double GetHeight(
            double latitude,
            double longitude) const;

        TerrainChunk GenerateChunk(
            std::int32_t chunkX,
            std::int32_t chunkY,
            double chunkSizeMeters,
            int resolution) const;

    private:
        std::uint64_t Seed = 0;
        double PlanetRadius = 0.0;
        double MaxHeight = 0.0;
        int Resolution = 64;

        double Noise(
            double x,
            double y) const;
    };
}
