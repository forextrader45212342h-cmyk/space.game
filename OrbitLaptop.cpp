#include "OrbitLaptop.h"

namespace Orbit
{
    void OrbitLaptop::Open()
    {
        if (State ==
            LaptopState::Closed)
        {
            State =
                LaptopState::Booting;

            BootTimer = 0.0;
        }
    }

    void OrbitLaptop::Close()
    {
        State =
            LaptopState::Closed;

        BootTimer = 0.0;
    }

    void OrbitLaptop::PowerOn()
    {
        State =
            LaptopState::Booting;

        BootTimer = 0.0;
    }

    void OrbitLaptop::Shutdown()
    {
        State =
            LaptopState::Shutdown;
    }

    void OrbitLaptop::Update(
        double deltaSeconds)
    {
        if (State !=
            LaptopState::Booting)
            return;

        BootTimer += deltaSeconds;

        if (BootTimer >= 3.0)
        {
            State =
                LaptopState::Ready;
        }
    }

    void OrbitLaptop::SetOwner(
        const std::string& owner)
    {
        Owner = owner;
    }

    bool OrbitLaptop::IsUsable() const
    {
        return State ==
            LaptopState::Ready;
    }

    LaptopState
    OrbitLaptop::GetState() const
    {
        return State;
    }

    const std::string&
    OrbitLaptop::GetOwner() const
    {
        return Owner;
    }
}
