#pragma once

#include <string>

namespace Orbit
{
    enum class LaptopState
    {
        Closed,
        Booting,
        Ready,
        Sleeping,
        Shutdown
    };

    class OrbitLaptop
    {
    public:
        void Open();

        void Close();

        void PowerOn();

        void Shutdown();

        void Update(
            double deltaSeconds);

        void SetOwner(
            const std::string& owner);

        bool IsUsable() const;

        LaptopState GetState() const;

        const std::string&
            GetOwner() const;

    private:
        LaptopState State =
            LaptopState::Closed;

        std::string Owner;
        double BootTimer = 0.0;
    };
}
