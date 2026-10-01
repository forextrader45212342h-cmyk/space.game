#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWildlifeManager.generated.h"

USTRUCT(BlueprintType)
struct FOrbitWildlifeAgent
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<AActor> Actor = nullptr;

    UPROPERTY()
    FVector Target = FVector::ZeroVector;

    UPROPERTY()
    float Speed = 200.0f;

    UPROPERTY()
    float SeparationRadius = 250.0f;
};

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitWildlifeManager
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWildlifeManager();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Wildlife")
    float UpdateInterval = 0.10f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Wildlife")
    float WanderRadius = 3000.0f;

    UFUNCTION(BlueprintCallable, Category="Wildlife")
    void RegisterWildlife(
        AActor* Actor,
        float Speed);

    UFUNCTION(BlueprintCallable, Category="Wildlife")
    void UnregisterWildlife(
        AActor* Actor);

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction*
        ThisTickFunction) override;

private:
    TArray<FOrbitWildlifeAgent> Agents;

    float Accumulator = 0.0f;

    void UpdateAgents(
        float DeltaSeconds);
};
