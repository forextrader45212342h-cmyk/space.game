#include "OrbitWorldInteraction.h"

namespace Orbit
{
    void OrbitWorldInteraction::Update(
        double deltaSeconds)
    {
        PlayerPhone.Update(deltaSeconds);
        HomeLaptop.Update(deltaSeconds);

        TrainSystem.Update(deltaSeconds);

        AircraftSystem.Update(
            deltaSeconds);
    }

    OrbitSmartphone&
    OrbitWorldInteraction::Phone()
    {
        return PlayerPhone;
    }

    OrbitLaptop&
    OrbitWorldInteraction::Laptop()
    {
        return HomeLaptop;
    }

    OrbitPlayerInventory&
    OrbitWorldInteraction::Inventory()
    {
        return PlayerInventory;
    }

    OrbitTrainSystem&
    OrbitWorldInteraction::Trains()
    {
        return TrainSystem;
    }

    OrbitRescueSystem&
    OrbitWorldInteraction::Rescue()
    {
        return RescueSystem;
    }

    OrbitAircraftSystem&
    OrbitWorldInteraction::Aircraft()
    {
        return AircraftSystem;
    }
}
