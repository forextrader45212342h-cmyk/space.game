#include "OrbitShipSystemsComponent.h"
#include "Math/UnrealMathUtility.h"

UOrbitShipSystemsComponent::UOrbitShipSystemsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitShipSystemsComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UOrbitShipSystemsComponent::UpdateSystems(
    float DeltaSeconds,
    float Throttle01,
    bool bCrewed)
{
    const float Throttle =
        FMath::Clamp(Throttle01, 0.0f, 1.0f);

    Fuel = FMath::Max(
        0.0f,
        Fuel -
        (0.015f + Throttle * 0.10f) *
        DeltaSeconds);

    if (bCrewed)
    {
        Oxygen = FMath::Max(
            0.0f,
            Oxygen -
            0.004f * DeltaSeconds);
    }

    const float TargetTemp =
        20.0f + Throttle * 120.0f;

    Temperature =
        FMath::FInterpTo(
            Temperature,
            TargetTemp,
            DeltaSeconds,
            1.5f);

    Power =
        FMath::FInterpTo(
            Power,
            Throttle > 0.02f
                ? 100.0f
                : 92.0f,
            DeltaSeconds,
            1.0f);

    if (Temperature > 115.0f)
    {
        Hull =
            FMath::Max(
                0.0f,
                Hull -
                (Temperature - 115.0f) *
                0.002f *
                DeltaSeconds);
    }
}

void UOrbitShipSystemsComponent::Refuel(
    float Amount)
{
    Fuel =
        FMath::Clamp(
            Fuel +
            FMath::Max(0.0f, Amount),
            0.0f,
            MaxFuel);
}

void UOrbitShipSystemsComponent::Repair(
    float Amount)
{
    Hull =
        FMath::Clamp(
            Hull +
            FMath::Max(0.0f, Amount),
            0.0f,
            MaxHull);
}

bool UOrbitShipSystemsComponent::IsOperational() const
{
    return
        Hull > 0.0f &&
        Power > 0.0f &&
        Fuel > 0.0f;
}
