#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitShipSystemsComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitShipSystemsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitShipSystemsComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Resources")
    float Fuel = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Resources")
    float Oxygen = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Damage")
    float Hull = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Power")
    float Power = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Thermal")
    float Temperature = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Limits")
    float MaxFuel = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Limits")
    float MaxOxygen = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ship|Limits")
    float MaxHull = 100.0f;

    UFUNCTION(BlueprintCallable, Category="Ship")
    void UpdateSystems(float DeltaSeconds, float Throttle01, bool bCrewed);

    UFUNCTION(BlueprintCallable, Category="Ship")
    void Refuel(float Amount);

    UFUNCTION(BlueprintCallable, Category="Ship")
    void Repair(float Amount);

    UFUNCTION(BlueprintCallable, Category="Ship")
    bool IsOperational() const;

protected:
    virtual void BeginPlay() override;
};
