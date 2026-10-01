#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitOceanComponent.generated.h"

USTRUCT(BlueprintType)
struct FOrbitWaveLayer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmplitudeCm = 80.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WavelengthCm = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpeedCmPerSecond = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction = FVector(1, 0, 0);
};

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitOceanComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitOceanComponent();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Ocean")
    TArray<FOrbitWaveLayer> Waves;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Ocean")
    float GlobalWaveScale = 1.0f;

    UFUNCTION(BlueprintCallable, Category="Ocean")
    float GetSurfaceHeightCm(
        const FVector& WorldLocation,
        float TimeSeconds) const;

    UFUNCTION(BlueprintCallable, Category="Ocean")
    FVector GetSurfaceNormal(
        const FVector& WorldLocation,
        float TimeSeconds) const;

protected:
    virtual void BeginPlay() override;
};
