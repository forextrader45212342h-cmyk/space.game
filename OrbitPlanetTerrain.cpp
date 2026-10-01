#include "OrbitPlanetTerrain.h"

#include <cmath>

namespace Orbit
{
    double OrbitPlanetTerrain::Noise(
        double x,
        double y) const
    {
        std::uint64_t n =
            Seed ^
            static_cast<std::uint64_t>(
                std::llround(x * 10000.0));

        n ^= static_cast<std::uint64_t>(
            std::llround(y * 10000.0)) *
            0x9E3779B97F4A7C15ULL;

        n ^= n >> 30;
        n *= 0xBF58476D1CE4E5B9ULL;
        n ^= n >> 27;
        n *= 0x94D049BB133111EBULL;
        n ^= n >> 31;

        return
            static_cast<double>(n % 1000000ULL) /
            1000000.0;
    }

    void OrbitPlanetTerrain::Generate(
        std::uint64_t seed,
        double planetRadiusMeters,
        double maxTerrainHeightMeters,
        int resolution)
    {
        Seed = seed;
        PlanetRadius = planetRadiusMeters;
        MaxHeight = maxTerrainHeightMeters;
        Resolution = resolution;
    }

    double OrbitPlanetTerrain::GetHeight(
        double latitude,
        double longitude) const
    {
        double value = 0.0;
        double amplitude = 1.0;
        double frequency = 1.0;

        for (int i = 0; i < 8; ++i)
        {
            value +=
                Noise(
                    latitude * frequency,
                    longitude * frequency)
                * amplitude;

            amplitude *= 0.5;
            frequency *= 2.0;
        }

        value /= 1.9921875;

        return
            (value - 0.5) *
            2.0 *
            MaxHeight;
    }

    TerrainChunk OrbitPlanetTerrain::GenerateChunk(
        std::int32_t chunkX,
        std::int32_t chunkY,
        double chunkSizeMeters,
        int resolution) const
    {
        TerrainChunk chunk;

        chunk.X = chunkX;
        chunk.Y = chunkY;
        chunk.Resolution = resolution;
        chunk.SizeMeters = chunkSizeMeters;

        chunk.Vertices.reserve(
            static_cast<std::size_t>(
                resolution * resolution));

        for (int y = 0; y < resolution; ++y)
        {
            for (int x = 0; x < resolution; ++x)
            {
                const double u =
                    static_cast<double>(x) /
                    static_cast<double>(resolution - 1);

                const double v =
                    static_cast<double>(y) /
                    static_cast<double>(resolution - 1);

                const double latitude =
                    (static_cast<double>(chunkY) + v)
                    * 0.01;

                const double longitude =
                    (static_cast<double>(chunkX) + u)
                    * 0.01;

                const double height =
                    GetHeight(
                        latitude,
                        longitude);

                TerrainVertex vertex;

                vertex.Height = height;

                vertex.Position = {
                    (u - 0.5) * chunkSizeMeters,
                    height,
                    (v - 0.5) * chunkSizeMeters
                };

                vertex.Normal = {
                    0.0,
                    1.0,
                    0.0
                };

                chunk.Vertices.push_back(vertex);
            }
        }

        for (int y = 0; y < resolution - 1; ++y)
        {
            for (int x = 0; x < resolution - 1; ++x)
            {
                const std::uint32_t i =
                    static_cast<std::uint32_t>(
                        y * resolution + x);

                chunk.Indices.push_back(i);
                chunk.Indices.push_back(i + 1);
                chunk.Indices.push_back(
                    i + resolution);

                chunk.Indices.push_back(i + 1);
                chunk.Indices.push_back(
                    i + resolution + 1);
                chunk.Indices.push_back(
                    i + resolution);
            }
        }

        chunk.Loaded = true;

        return chunk;
    }
}
