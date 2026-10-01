#pragma once

#include "OrbitSmartphone.h"
#include "OrbitLaptop.h"
#include "OrbitPlayerInventory.h"
#include "OrbitTrainSystem.h"
#include "OrbitRescueSystem.h"
#include "OrbitAircraftSystem.h"

namespace Orbit
{
    class OrbitWorldInteraction
    {
    public:
        void Update(double deltaSeconds);

        OrbitSmartphone& Phone();
        OrbitLaptop& Laptop();
        OrbitPlayerInventory& Inventory();
        OrbitTrainSystem& Trains();
        OrbitRescueSystem& Rescue();
        OrbitAircraftSystem& Aircraft();

    private:
        OrbitSmartphone PlayerPhone;
        OrbitLaptop HomeLaptop;
        OrbitPlayerInventory PlayerInventory;

        OrbitTrainSystem TrainSystem;
        OrbitRescueSystem RescueSystem;
        OrbitAircraftSystem AircraftSystem;
    };
}
