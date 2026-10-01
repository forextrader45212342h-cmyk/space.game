#pragma once

#include "OrbitPBR.h"

#include <vector>
#include <cstdint>

namespace OrbitParticles
{
    struct Particle
    {
        OrbitPBR::Vec3 position{};
        OrbitPBR::Vec3 velocity{};

        float lifetime = 1.0f;
        float age = 0.0f;

        float size = 1.0f;

        OrbitPBR::Color color{1,1,1,1};

        bool alive = false;
    };

    class System
    {
    private:
        std::vector<Particle> particles;

        uint32_t maximum = 0;

    public:
        bool Initialize(uint32_t maxParticles);

        void Update(float deltaSeconds);

        uint32_t Emit(
            const Particle& particle);

        void Clear();

        uint32_t AliveCount() const;

        uint32_t Capacity() const;

        const std::vector<Particle>&
        GetParticles() const;
    };
}
