// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponSystem.generated.h"

USTRUCT(BlueprintType)
struct FLaserTelemetry
{
    GENERATED_BODY()

    /** Start location of the laser trace */
    UPROPERTY(BlueprintReadOnly)
    FVector Start;

    /** End location of the laser trace */
    UPROPERTY(BlueprintReadOnly)
    FVector End;

    /** Whether the trace hit something */
    UPROPERTY(BlueprintReadOnly)
    bool bHit;

    /** Actor that was hit (if any) */
    UPROPERTY(BlueprintReadOnly)
    AActor* HitActor;

    /** Damage applied to the hit actor */
    UPROPERTY(BlueprintReadOnly)
    float Damage;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponSystem();

    /** Fire a laser from the owning actor's muzzle */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    void FireLaser();

    /** Get the telemetry data for the last shot */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    const FLaserTelemetry& GetLastTelemetry() const { return LastTelemetry; }

protected:
    virtual void BeginPlay() override;

private:
    /** Damage dealt by a single laser hit */
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    float LaserDamage = 25.0f;

    /** Maximum range of the laser */
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    float LaserRange = 10000.0f;

    /** Channel used for the line trace */
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

    /** Telemetry for the most recent shot */
    FLaserTelemetry LastTelemetry;
};
