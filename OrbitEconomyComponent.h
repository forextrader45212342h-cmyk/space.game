#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitEconomyComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitEconomyComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitEconomyComponent();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        SaveGame,
        Category="Economy")
    int64 Cash = 0;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        SaveGame,
        Category="Economy")
    int64 LifetimeEarned = 0;

    UFUNCTION(BlueprintCallable, Category="Economy")
    void AddReward(int64 Amount);

    UFUNCTION(BlueprintCallable, Category="Economy")
    bool TrySpend(int64 Amount);

    UFUNCTION(BlueprintPure, Category="Economy")
    bool CanAfford(int64 Amount) const;
};
