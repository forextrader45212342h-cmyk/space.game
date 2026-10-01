#include "OrbitalSatelliteComponent.h"

UOrbitalSatelliteComponent::
UOrbitalSatelliteComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void
UOrbitalSatelliteComponent::
BeginPlay()
{
    Super::BeginPlay();
}

void
UOrbitalSatelliteComponent::
SetOrbit(
    double RadiusCm,
    double PeriodSeconds)
{
    OrbitRadiusCm =
        FMath::Max(
            1000.0,
            RadiusCm);

    OrbitalPeriodSeconds =
        FMath::Max(
            1.0,
            PeriodSeconds);
}

void
UOrbitalSatelliteComponent::
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

    if (!GetOwner())
        return;

    ElapsedSeconds +=
        DeltaTime;

    const double AngularVelocity =
        2.0 * PI /
        OrbitalPeriodSeconds;

    const double Angle =
        FMath::DegreesToRadians(
            static_cast<double>(
                PhaseDegrees))
        +
        AngularVelocity *
        ElapsedSeconds;

    const double X =
        FMath::Cos(Angle) *
        OrbitRadiusCm;

    const double Y =
        FMath::Sin(Angle) *
        OrbitRadiusCm;

    const double Inclination =
        FMath::DegreesToRadians(
            static_cast<double>(
                InclinationDegrees));

    const double Z =
        Y *
        FMath::Sin(
            Inclination);

    const double YRot =
        Y *
        FMath::Cos(
            Inclination);

    const FVector Position =
        PlanetCenterCm +
        FVector(
            X,
            YRot,
            Z);

    GetOwner()->SetActorLocation(
        Position,
        false);
}
