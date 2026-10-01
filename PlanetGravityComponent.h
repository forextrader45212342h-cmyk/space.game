#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlanetGravityComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UPlanetGravityComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UPlanetGravityComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    FVector PlanetCenter = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    double PlanetRadius = 637100000.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    double SurfaceGravity = 9.81;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    double GravityFalloffStart = 10000.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    double GravityFalloffEnd = 1000000.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
    bool bGravityEnabled = true;

    UFUNCTION(BlueprintCallable, Category="Gravity")
    FVector GetGravityVector() const;

    UFUNCTION(BlueprintCallable, Category="Gravity")
    double GetAltitude() const;

    UFUNCTION(BlueprintCallable, Category="Gravity")
    double GetGravityStrength() const;

    UFUNCTION(BlueprintCallable, Category="Gravity")
    FVector GetGravityUpVector() const;

    UFUNCTION(BlueprintCallable, Category="Gravity")
    void SetGravityEnabled(bool bEnabled);

private:

    double CalculateGravityFalloff(double Altitude) const;
};
