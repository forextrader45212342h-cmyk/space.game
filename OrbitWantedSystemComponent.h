#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWantedSystemComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitWantedSystemComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWantedSystemComponent();

    UPROPERTY(
        BlueprintReadOnly,
        Category="Law")
    int32 WantedLevel = 0;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Law")
    int32 MaxWantedLevel = 5;

    UPROPERTY(
        BlueprintReadOnly,
        Category="Law")
    bool bWanted = false;

    UFUNCTION(BlueprintCallable, Category="Law")
    void RegisterCrime(float Severity);

    UFUNCTION(BlueprintCallable, Category="Law")
    void ClearWanted();

    UFUNCTION(BlueprintPure, Category="Law")
    FVector GetTargetVector(
        const FVector& PoliceLocation) const;

protected:
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction*
        ThisTickFunction) override;
};
