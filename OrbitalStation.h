#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitalStation.generated.h"

UCLASS()
class PROJECTORBIT_API AOrbitalStation
    : public AActor
{
    GENERATED_BODY()

public:
    AOrbitalStation();

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly)
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly)
    TObjectPtr<USceneComponent> StationBody;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly)
    TArray<TObjectPtr<USceneComponent>>
        DockingPorts;

    UFUNCTION(BlueprintCallable, Category="Station")
    USceneComponent* GetNearestDockingPort(
        const FVector& WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category="Station")
    bool IsDockingAlignmentValid(
        const FVector& ShipLocation,
        const FVector& ShipForward) const;

protected:
    virtual void BeginPlay() override;
};
