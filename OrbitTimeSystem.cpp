#include "OrbitTimeSystem.h"

namespace Orbit
{
    void OrbitTimeSystem::Reset()
    {
        SimulationSeconds = 0.0;
        TimeScale = 1.0;
        Paused = false;
    }

    void OrbitTimeSystem::SetTimeScale(double scale)
    {
        if (scale < 0.0)
            scale = 0.0;

        TimeScale = scale;
    }

    double OrbitTimeSystem::GetTimeScale() const
    {
        return TimeScale;
    }

    void OrbitTimeSystem::Pause(bool paused)
    {
        Paused = paused;
    }

    bool OrbitTimeSystem::IsPaused() const
    {
        return Paused;
    }

    void OrbitTimeSystem::Tick(double realDeltaSeconds)
    {
        if (Paused)
            return;

        if (realDeltaSeconds < 0.0)
            return;

        SimulationSeconds += realDeltaSeconds * TimeScale;
    }

    double OrbitTimeSystem::GetSimulationSeconds() const
    {
        return SimulationSeconds;
    }
}
