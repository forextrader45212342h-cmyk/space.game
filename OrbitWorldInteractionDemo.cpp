#include "OrbitWorldInteraction.h"

#include <iostream>

int main()
{
    Orbit::OrbitWorldInteraction world;

    // PHONE
    world.Phone().AddContact({
        1,
        "Mountain Rescue",
        "4433222"
    });

    world.Phone().Call("4433222");

    // LAPTOP
    world.Laptop().SetOwner("Player");
    world.Laptop().Open();

    // INVENTORY
    world.Inventory().AddItem({
        100,
        "Smartphone",
        1,
        0.2,
        Orbit::ItemLocation::Pocket,
        true
    });

    world.Inventory().AddItem({
        101,
        "Laptop",
        1,
        1.5,
        Orbit::ItemLocation::Home,
        true
    });

    // TRAIN STATIONS
    world.Trains().AddStation({
        1,
        "City Central",
        {0.0, 0.0, 0.0}
    });

    world.Trains().AddStation({
        2,
        "Mountain Station",
        {10000.0, 0.0, 8000.0}
    });

    Orbit::Train train;

    train.Id = 1;
    train.Name = "ORBIT EXPRESS";
    train.Position = {0.0, 0.0, 0.0};
    train.Route = {1, 2};
    train.MaximumSpeed = 100.0;

    world.Trains().AddTrain(train);

    world.Trains().StartTrain(1);

    // LOST PERSON
    world.Rescue().AddLostPerson({
        500,
        "Lost Hiker",
        {10000.0, 0.0, 8000.0},
        80.0,
        Orbit::RescueStatus::Waiting
    });

    world.Rescue().StartSearch(500);

    // AIRCRAFT
    Orbit::Aircraft plane;

    plane.Id = 77;
    plane.Name = "ORBIT RESCUE AIR-01";
    plane.Position = {
        5000.0,
        5000.0,
        5000.0
    };
    plane.Velocity = {
        1.0,
        -0.2,
        0.5
    };
    plane.CruiseSpeed = 250.0;

    world.Aircraft().AddAircraft(plane);

    // Airdrop
    world.Aircraft().AssignAirdrop(
        77,
        {
            900,
            "Emergency Rescue Package",
            25.0,
            {10000.0, 0.0, 8000.0},
            false,
            false
        });

    for (int i = 0; i < 100; ++i)
    {
        world.Update(0.1);
    }

    std::cout
        << "PROJECT ORBIT WORLD SYSTEM ONLINE\n";

    std::cout
        << "Phone number: "
        << world.Phone()
            .GetCurrentCall()
            .Number
        << "\n";

    std::cout
        << "Laptop usable: "
        << world.Laptop().IsUsable()
        << "\n";

    std::cout
        << "Trains: "
        << world.Trains()
            .GetTrains()
            .size()
        << "\n";

    std::cout
        << "Aircraft: "
        << world.Aircraft()
            .GetAircraft()
            .size()
        << "\n";

    return 0;
}
