#include "OrbitWantedSystemComponent.h"

UOrbitWantedSystemComponent::
UOrbitWantedSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitWantedSystemComponent::RegisterCrime(
    float Severity)
{
    if (Severity <= 0.0f)
        return;

    const int32 Added =
        FMath::Max(
            1,
            FMath::RoundToInt(
                Severity));

    WantedLevel =
        FMath::Clamp(
            WantedLevel + Added,
            0,
            MaxWantedLevel);

    bWanted =
        WantedLevel > 0;
}

void UOrbitWantedSystemComponent::ClearWanted()
{
    WantedLevel = 0;

    bWanted = false;
}

FVector
UOrbitWantedSystemComponent::
GetTargetVector(
    const FVector& PoliceLocation) const
{
    const AActor* OwnerActor =
        GetOwner();

    if (!OwnerActor)
        return FVector::ZeroVector;

    return
        OwnerActor->GetActorLocation()
        -
        PoliceLocation;
}

void UOrbitWantedSystemComponent::
TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction*
    ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction);
}
