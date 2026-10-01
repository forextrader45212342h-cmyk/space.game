#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleDynamicsComponent.generated.h"

UCLASS(ClassGroup=(ProjectOrbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitVehicleDynamicsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitVehicleDynamicsComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smooth Dynamics")
    float MaxForwardSpeedCmPerSec = 12000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smooth Dynamics", meta=(ClampMin="0.1"))
    float AccelerationResponse = 3.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smooth Dynamics", meta=(ClampMin="0.1"))
    float DecelerationResponse = 5.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smooth Dynamics")
    float ReverseSpeedFraction = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Smooth Dynamics")
    bool bAffectOnlyForwardAxis = true;

    UPROPERTY(BlueprintReadOnly, Category="Smooth Dynamics")
    FVector SmoothedVelocity = FVector::ZeroVector;

    UFUNCTION(BlueprintCallable, Category="Smooth Dynamics")
    void SetThrottle(float NewThrottle);

    UFUNCTION(BlueprintCallable, Category="Smooth Dynamics")
    void SetBraking(float NewBrake);

    UFUNCTION(BlueprintPure, Category="Smooth Dynamics")
    FVector GetSmoothedVelocity() const
    {
        return SmoothedVelocity;
    }

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:
    float ThrottleInput = 0.0f;
    float BrakeInput = 0.0f;
};
