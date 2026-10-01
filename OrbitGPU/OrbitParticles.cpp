#include "OrbitParticles.h"

namespace OrbitParticles
{
    bool System::Initialize(
        uint32_t maxParticles)
    {
        if (maxParticles == 0)
            return false;

        maximum = maxParticles;
        particles.clear();
        particles.resize(maximum);

        return true;
    }

    void System::Update(float deltaSeconds)
    {
        if (deltaSeconds < 0)
            return;

        for (auto& particle : particles)
        {
            if (!particle.alive)
                continue;

            particle.age += deltaSeconds;

            if (particle.age >= particle.lifetime)
            {
                particle.alive = false;
                continue;
            }

            particle.position.x +=
                particle.velocity.x * deltaSeconds;

            particle.position.y +=
                particle.velocity.y * deltaSeconds;

            particle.position.z +=
                particle.velocity.z * deltaSeconds;
        }
    }

    uint32_t System::Emit(
        const Particle& particle)
    {
        for (uint32_t i = 0;
             i < particles.size();
             ++i)
        {
            if (!particles[i].alive)
            {
                particles[i] = particle;
                particles[i].alive = true;
                particles[i].age = 0.0f;

                return i;
            }
        }

        return UINT32_MAX;
    }

    void System::Clear()
    {
        for (auto& particle : particles)
            particle.alive = false;
    }

    uint32_t System::AliveCount() const
    {
        uint32_t count = 0;

        for (const auto& particle : particles)
        {
            if (particle.alive)
                ++count;
        }

        return count;
    }

    uint32_t System::Capacity() const
    {
        return maximum;
    }

    const std::vector<Particle>&
    System::GetParticles() const
    {
        return particles;
    }
}
