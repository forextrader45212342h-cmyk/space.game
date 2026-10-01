#include "OrbitalStation.h"

AOrbitalStation::AOrbitalStation()
{
    PrimaryActorTick.bCanEverTick = false;

    Root =
        CreateDefaultSubobject<USceneComponent>(
            TEXT("Root"));

    RootComponent =
        Root;

    StationBody =
        CreateDefaultSubobject<USceneComponent>(
            TEXT("StationBody"));

    StationBody->SetupAttachment(
        Root);

    for (int32 i = 0; i < 6; ++i)
    {
        USceneComponent* Port =
            CreateDefaultSubobject<USceneComponent>(
                *FString::Printf(
                    TEXT("DockingPort_%d"),
                    i));

        Port->SetupAttachment(
            Root);

        const float Angle =
            static_cast<float>(i) *
            60.0f;

        Port->SetRelativeLocation(
            FVector(
                FMath::Cos(
                    FMath::DegreesToRadians(
                        Angle))
                * 1000.0f,

                FMath::Sin(
                    FMath::DegreesToRadians(
                        Angle))
                * 1000.0f,

                0.0f));

        Port->SetRelativeRotation(
            FRotator(
                0.0f,
                Angle + 180.0f,
                0.0f));

        DockingPorts.Add(
            Port);
    }
}

void AOrbitalStation::BeginPlay()
{
    Super::BeginPlay();
}

USceneComponent*
AOrbitalStation::
GetNearestDockingPort(
    const FVector& WorldLocation) const
{
    USceneComponent* Best = nullptr;

    double BestDistance =
        TNumericLimits<double>::Max();

    for (USceneComponent* Port :
        DockingPorts)
    {
        if (!Port)
            continue;

        const double Distance =
            FVector::DistSquared(
                Port->GetComponentLocation(),
                WorldLocation);

        if (Distance < BestDistance)
        {
            BestDistance =
                Distance;

            Best = Port;
        }
    }

    return Best;
}

bool
AOrbitalStation::
IsDockingAlignmentValid(
    const FVector& ShipLocation,
    const FVector& ShipForward) const
{
    USceneComponent* Port =
        GetNearestDockingPort(
            ShipLocation);

    if (!Port)
        return false;

    const float Distance =
        FVector::Dist(
            ShipLocation,
            Port->GetComponentLocation());

    const float Alignment =
        FVector::DotProduct(
            ShipForward.GetSafeNormal(),
            Port->GetForwardVector());

    return
        Distance <= 500.0f &&
        Alignment >= 0.96f;
}
