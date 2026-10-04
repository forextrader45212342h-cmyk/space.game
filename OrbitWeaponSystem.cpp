// OrbitWeaponSystem.cpp
// UE5 C++ implementation of a laser weapon system that performs line‑traces,
// applies damage, records telemetry, and spawns impact effects.

#include "OrbitWeaponSystem.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/DamageType.h"
#include "Logging/LogMacros.h"

// Log category for telemetry
DEFINE_LOG_CATEGORY_STATIC(LogOrbitWeapon, Log, All);

// -----------------------------------------------------------------------------
// UOrbitWeaponSystem
// -----------------------------------------------------------------------------
UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default values
    LaserRange = 10000.f;
    DamageAmount = 25.f;
    bAutoFire = false;
    FireRate = 0.2f; // 5 shots per second
    ImpactEffect = nullptr;
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoFire)
    {
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UOrbitWeaponSystem::FireLaser, FireRate, true);
    }
}

void UOrbitWeaponSystem::FireLaser()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // Determine start and end points
    FVector MuzzleLocation = Owner->GetActorLocation();
    FRotator MuzzleRotation = Owner->GetActorRotation();
    FVector End = MuzzleLocation + MuzzleRotation.Vector() * LaserRange;

    // Perform line trace
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        MuzzleLocation,
        End,
        ECC_Visibility,
        QueryParams
    );

    // Debug line
    DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 1.f, 0, 1.f);

    // Telemetry: log the shot
    Telemetry_LogShot(MuzzleLocation, End, bHit, HitResult);

    if (bHit)
    {
        // Apply damage
        UGameplayStatics::ApplyPointDamage(
            HitResult.GetActor(),
            DamageAmount,
            MuzzleRotation.Vector(),
            HitResult,
            Owner->GetInstigatorController(),
            Owner,
            UDamageType::StaticClass()
        );

        // Spawn impact effect
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                ImpactEffect,
                HitResult.ImpactPoint,
                HitResult.ImpactNormal.Rotation(),
                true
            );
        }
    }
}

void UOrbitWeaponSystem::Telemetry_LogShot(const FVector& Start, const FVector& End, bool bHit, const FHitResult& Hit)
{
    // Simple telemetry: log to console and optionally send to a telemetry manager
    FString HitInfo = bHit
        ? FString::Printf(TEXT("Hit %s at %s"), *Hit.GetActor()->GetName(), *Hit.ImpactPoint.ToString())
        : TEXT("No hit");

    UE_LOG(LogOrbitWeapon, Log, TEXT("Laser fired from %s to %s. %s"),
        *Start.ToString(), *End.ToString(), *HitInfo);

    // Example of sending data to a telemetry system (pseudo-code)
    // TelemetryManager::Get()->RecordEvent(TEXT("LaserShot"), {
    //     { TEXT("Start"), Start },
    //     { TEXT("End"), End },
    //     { TEXT("Hit"), bHit },
    //     { TEXT("HitActor"), bHit ? Hit.GetActor()->GetName() : TEXT("") }
    // });
}

// -----------------------------------------------------------------------------
// UOrbitWeaponSystem.h
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponSystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOURGAME_API UOrbitWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponSystem();

    /** Fires a laser beam. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void FireLaser();

protected:
    virtual void BeginPlay() override;

private:
    /** Telemetry helper. */
    void Telemetry_LogShot(const FVector& Start, const FVector& End, bool bHit, const FHitResult& Hit);

    /** Timer handle for auto‑fire. */
    FTimerHandle FireTimerHandle;

    /** Range of the laser. */
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float LaserRange;

    /** Damage dealt per hit. */
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float DamageAmount;

    /** Whether the weapon should auto‑fire. */
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    bool bAutoFire;

    /** Fire rate in seconds. */
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float FireRate;

    /** Particle system to spawn on impact. */
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* ImpactEffect;
};
