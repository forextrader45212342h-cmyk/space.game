// OrbitWeaponSystem.h
#pragma once

// -----------------------------------------------------------------------------
//  OrbitWeaponSystem
//
//  A lightweight UE5 component that implements a laser‑weapon system.  It
//  performs a ray‑cast to detect hits, spawns a visual projectile (a
//  simple particle system), and enforces a per‑weapon cooldown.
//
//  The component is intentionally lightweight – it can be dropped into any
//  Actor that owns a weapon.  It uses UE5's built‑in physics and
//  particle systems, so no custom rendering code is required.
//
//  Usage example:
//
//      // In your character or pawn
//      UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//      OrbitWeaponSystem* WeaponSystem = CreateDefaultSubobject<OrbitWeaponSystem>(TEXT("WeaponSystem"));
//
//      // Fire from a socket or location
//      WeaponSystem->FireWeapon(GetActorLocation(), GetActorForwardVector());
//
// -----------------------------------------------------------------------------

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "OrbitWeaponSystem.generated.h"

USTRUCT(BlueprintType)
struct FWeaponConfig
{
    GENERATED_BODY()

    // Maximum distance the laser can travel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float MaxRange = 10000.f;

    // Time between consecutive shots (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float CooldownTime = 0.5f;

    // Particle system to spawn for the projectile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* ProjectileTemplate = nullptr;

    // Optional: damage dealt on hit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 25.f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponSystem();

    /** Fires the weapon from the given start location in the given direction. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void FireWeapon(const FVector& StartLocation, const FVector& Direction);

    /** Returns true if the weapon is currently on cooldown. */
    UFUNCTION(BlueprintPure, Category = "Weapon")
    bool IsOnCooldown() const { return bIsCoolingDown; }

    /** Sets the weapon configuration. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponConfig(const FWeaponConfig& NewConfig) { Config = NewConfig; }

protected:
    virtual void BeginPlay() override;

private:
    /** Handles the cooldown timer. */
    void ResetCooldown();

    /** Performs a ray‑cast and returns the hit result. */
    bool PerformRaycast(const FVector& Start, const FVector& End, FHitResult& OutHit) const;

    /** Spawns the projectile visual effect. */
    void SpawnProjectile(const FVector& Start, const FVector& End);

    /** Applies damage to the hit actor if it implements the damage interface. */
    void ApplyDamage(const FHitResult& Hit) const;

private:
    /** Weapon configuration. */
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FWeaponConfig Config;

    /** Whether the weapon is currently cooling down. */
    bool bIsCoolingDown = false;

    /** Timer handle for the cooldown. */
    FTimerHandle CooldownTimerHandle;
};
