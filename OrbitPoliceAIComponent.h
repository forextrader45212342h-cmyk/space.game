#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitPoliceAIComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitPoliceAIComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitPoliceAIComponent();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Police")
    float SearchRadius = 50000.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Police")
    float CaptureDistance = 180.0f;

    UPROPERTY(
        BlueprintReadOnly,
        Category="Police")
    bool bDispatched = false;

    UFUNCTION(BlueprintCallable, Category="Police")
    void Dispatch(AActor* TargetActor);

    UFUNCTION(BlueprintCallable, Category="Police")
    bool IsTargetCaptured() const;

    UFUNCTION(BlueprintPure, Category="Police")
    FVector GetTargetLocation() const;

protected:
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction*
        ThisTickFunction) override;

private:
    TWeakObjectPtr<AActor> Target;
};
