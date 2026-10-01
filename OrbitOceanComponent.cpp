#include "OrbitOceanComponent.h"

UOrbitOceanComponent::UOrbitOceanComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitOceanComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Waves.Num() == 0)
    {
        FOrbitWaveLayer A;

        A.AmplitudeCm = 70.0f;
        A.WavelengthCm = 1800.0f;
        A.SpeedCmPerSecond = 120.0f;
        A.Direction = FVector(1, 0, 0);

        FOrbitWaveLayer B;

        B.AmplitudeCm = 35.0f;
        B.WavelengthCm = 900.0f;
        B.SpeedCmPerSecond = 85.0f;
        B.Direction = FVector(0.4f, 1.0f, 0);

        Waves.Add(A);
        Waves.Add(B);
    }
}

float UOrbitOceanComponent::GetSurfaceHeightCm(
    const FVector& WorldLocation,
    float TimeSeconds) const
{
    float Height = 0.0f;

    for (const FOrbitWaveLayer& Wave : Waves)
    {
        const FVector2D Dir(
            Wave.Direction.X,
            Wave.Direction.Y);

        const FVector2D SafeDir =
            Dir.GetSafeNormal();

        const float K =
            2.0f * PI /
            FMath::Max(
                1.0f,
                Wave.WavelengthCm);

        const float Phase =
            FVector2D(
                WorldLocation.X,
                WorldLocation.Y
            ).Dot(SafeDir) * K
            -
            Wave.SpeedCmPerSecond *
            K *
            TimeSeconds;

        Height +=
            FMath::Sin(Phase) *
            Wave.AmplitudeCm *
            GlobalWaveScale;
    }

    return Height;
}

FVector UOrbitOceanComponent::GetSurfaceNormal(
    const FVector& WorldLocation,
    float TimeSeconds) const
{
    const float Sample = 50.0f;

    const float H0 =
        GetSurfaceHeightCm(
            WorldLocation,
            TimeSeconds);

    const float HX =
        GetSurfaceHeightCm(
            WorldLocation +
            FVector(Sample, 0, 0),
            TimeSeconds);

    const float HY =
        GetSurfaceHeightCm(
            WorldLocation +
            FVector(0, Sample, 0),
            TimeSeconds);

    return FVector(
        -(HX - H0) / Sample,
        -(HY - H0) / Sample,
        1.0f
    ).GetSafeNormal();
}
