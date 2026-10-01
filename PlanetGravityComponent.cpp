#include "Physics/PlanetGravityComponent.h"
#include "GameFramework/Actor.h"

UPlanetGravityComponent::UPlanetGravityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPlanetGravityComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPlanetGravityComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction
    );

    if (!bGravityEnabled)
    {
        return;
    }

    AActor* Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    UPrimitiveComponent* Primitive =
        Cast<UPrimitiveComponent>(
            Owner->GetRootComponent()
        );

    if (!Primitive)
    {
        return;
    }

    if (!Primitive->IsSimulatingPhysics())
    {
        return;
    }

    const FVector Gravity =
        GetGravityVector();

    Primitive->AddForce(
        Gravity,
        NAME_None,
        true
    );
}

double UPlanetGravityComponent::GetAltitude() const
{
    const AActor* Owner = GetOwner();

    if (!Owner)
    {
        return 0.0;
    }

    const double Distance =
        FVector::Dist(
            Owner->GetActorLocation(),
            PlanetCenter
        );

    return FMath::Max(
        0.0,
        Distance - PlanetRadius
    );
}

double UPlanetGravityComponent::CalculateGravityFalloff(
    double Altitude) const
{
    if (Altitude <= GravityFalloffStart)
    {
        return 1.0;
    }

    if (Altitude >= GravityFalloffEnd)
    {
        return 0.0;
    }

    const double Alpha =
        FMath::GetMappedRangeValueClamped(
            FVector2D(
                GravityFalloffStart,
                GravityFalloffEnd
            ),
            FVector2D(0.0,1.0),
            Altitude
        );

    return 1.0 - Alpha;
}

double UPlanetGravityComponent::GetGravityStrength() const
{
    const double Altitude =
        GetAltitude();

    const double Falloff =
        CalculateGravityFalloff(
            Altitude
        );

    const double Distance =
        PlanetRadius + Altitude;

    if (Distance <= 0.0)
    {
        return 0.0;
    }

    const double InverseSquare =
        FMath::Square(
            PlanetRadius / Distance
        );

    return SurfaceGravity *
           InverseSquare *
           Falloff;
}

FVector UPlanetGravityComponent::GetGravityVector() const
{
    const AActor* Owner = GetOwner();

    if (!Owner)
    {
        return FVector::ZeroVector;
    }

    const FVector Direction =
        (PlanetCenter -
         Owner->GetActorLocation())
        .GetSafeNormal();

    return Direction *
           GetGravityStrength();
}

FVector UPlanetGravityComponent::GetGravityUpVector() const
{
    const AActor* Owner = GetOwner();

    if (!Owner)
    {
        return FVector::UpVector;
    }

    return (
        Owner->GetActorLocation() -
        PlanetCenter
    ).GetSafeNormal();
}

void UPlanetGravityComponent::SetGravityEnabled(
    bool bEnabled)
{
    bGravityEnabled = bEnabled;
}
