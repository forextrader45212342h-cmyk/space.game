#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitBiomeEnvironmentComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitBiomeEnvironmentComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitBiomeEnvironmentComponent();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rainforest")
    float TreeDensity = 0.75f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rainforest")
    float Humidity = 0.90f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rainforest")
    float RainIntensity = 0.65f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Lighthouse")
    float LighthouseBeamAngleDegrees =
        35.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Lighthouse")
    float LighthouseRotationDegreesPerSecond =
        18.0f;

    UPROPERTY(
        BlueprintReadOnly,
        Category="Lighthouse")
    float LighthouseCurrentAngle =
        0.0f;

    UFUNCTION(BlueprintCallable, Category="Environment")
    void UpdateEnvironment(
        float DeltaSeconds,
        float TimeOfDay01);

protected:
    virtual void BeginPlay() override;
};
