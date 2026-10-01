#pragma once

namespace Orbit
{
    class OrbitTimeSystem
    {
    public:
        void Reset();

        void SetTimeScale(double scale);
        double GetTimeScale() const;

        void Pause(bool paused);
        bool IsPaused() const;

        void Tick(double realDeltaSeconds);

        double GetSimulationSeconds() const;

    private:
        double SimulationSeconds = 0.0;
        double TimeScale = 1.0;
        bool Paused = false;
    };
}
