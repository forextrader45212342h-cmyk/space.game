#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitAmbientMoverComponent.generated.h"

class USplineComponent;
class AActor;

UCLASS(ClassGroup=(ProjectOrbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitAmbientMoverComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:

    UOrbitAmbientMoverComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ambient Movement")
    TObjectPtr<USplineComponent> FollowSpline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ambient Movement")
    float SpeedCmPerSec = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ambient Movement")
    float LookAheadDistanceCm = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Streaming LOD")
    float ActiveDistanceCm = 250000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Streaming LOD")
    TObjectPtr<AActor> FocusActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Streaming LOD")
    bool bDisableCollisionWhenFar = true;

    UFUNCTION(BlueprintCallable, Category="Ambient Movement")
    void ResetToSplineStart();

protected:

    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:

    float DistanceAlongSpline = 0.0f;
    bool bWasFar = false;

    FCollisionResponseContainer SavedResponses;

    ECollisionEnabled::Type SavedCollision =
        ECollisionEnabled::QueryAndPhysics;
};
