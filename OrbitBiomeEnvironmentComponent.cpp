#include "OrbitBiomeEnvironmentComponent.h"

UOrbitBiomeEnvironmentComponent::
UOrbitBiomeEnvironmentComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void
UOrbitBiomeEnvironmentComponent::
BeginPlay()
{
    Super::BeginPlay();
}

void
UOrbitBiomeEnvironmentComponent::
UpdateEnvironment(
    float DeltaSeconds,
    float TimeOfDay01)
{
    LighthouseCurrentAngle +=
        LighthouseRotationDegreesPerSecond *
        DeltaSeconds;

    LighthouseCurrentAngle =
        FMath::Fmod(
            LighthouseCurrentAngle,
            360.0f);

    const float NightFactor =
        1.0f -
        FMath::Clamp(
            FMath::Sin(
                TimeOfDay01 *
                2.0f *
                PI),
            0.0f,
            1.0f);

    LighthouseBeamAngleDegrees =
        FMath::Clamp(
            20.0f +
            NightFactor *
            20.0f,
            10.0f,
            60.0f);
}
