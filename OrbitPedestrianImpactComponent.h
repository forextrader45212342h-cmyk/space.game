#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitPedestrianImpactComponent.generated.h"

UCLASS(ClassGroup=(ProjectOrbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitPedestrianImpactComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:

    UOrbitPedestrianImpactComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
    float KnockoutSpeedCmPerSec = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
    float MinimumImpactImpulse = 150000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
    float RecoveryTimeSeconds = 5.0f;

    UFUNCTION(BlueprintCallable, Category="Impact")
    bool HandleVehicleImpact(
        AActor* VehicleActor,
        FVector ImpactPoint,
        FVector VehicleVelocity
    );

    UFUNCTION(BlueprintPure, Category="Impact")
    bool IsUnconscious() const
    {
        return bUnconscious;
    }

private:

    void RecoverFromKnockdown();

    bool bUnconscious = false;

    FTimerHandle RecoveryTimer;
};
