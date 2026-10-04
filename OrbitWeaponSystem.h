// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  UOrbitWeaponSystem
 *  ------------------
 *  A reusable component that handles laser‑style ray‑cast weapons
 *  as well as projectile‑based weapons.  It supports:
 *      • Ray‑cast laser hit detection
 *      • Projectile spawning with optional damage
 *      • Cool‑down timer between shots
 *      • Configurable damage, range, trace channel, and visual effects
 *
 *  The component is intended to be attached to any Actor that
 *  should be able to fire a weapon (e.g. a player pawn or an AI
 *  turret).  All properties are exposed to Blueprints for easy
 *  tuning.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitWeaponSystem();

	/** Fire the weapon.  Returns true if a shot was fired. */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool FireWeapon();

protected:
	virtual void BeginPlay() override;

private:
	/** Perform a ray‑cast from the muzzle and apply damage if hit. */
	void PerformRaycast();

	/** Spawn a projectile actor at the muzzle location. */
	void SpawnProjectile();

	/** Start the cooldown timer. */
	void StartCooldown();

	/** Reset the ability to fire. */
	void ResetCooldown();

	/** Apply damage to the hit actor. */
	void ApplyDamage(const FHitResult& Hit);

	/** Helper to get the world location of the muzzle. */
	FVector GetMuzzleLocation() const;

	/** Helper to get the world rotation of the muzzle. */
	FRotator GetMuzzleRotation() const;

private:
	/** The class of the projectile to spawn.  If null, no projectile is spawned. */
	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	/** Optional particle system to spawn as a laser beam. */
	UPROPERTY(EditAnywhere, Category="Weapon|Laser")
	UParticleSystem* LaserBeamEffect;

	/** Damage applied to the hit actor. */
	UPROPERTY(EditAnywhere, Category="Weapon|Damage")
	float Damage = 25.f;

	/** Maximum range of the ray‑cast. */
	UPROPERTY(EditAnywhere, Category="Weapon|Range")
	float Range = 10000.f;

	/** Cool‑down time between shots in seconds. */
	UPROPERTY(EditAnywhere, Category="Weapon|Cooldown")
	float CooldownTime = 0.5f;

	/** Collision channel used for the ray‑cast. */
	UPROPERTY(EditAnywhere, Category="Weapon|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** Offset from the muzzle component along its forward axis. */
	UPROPERTY(EditAnywhere, Category="Weapon|Muzzle")
	float MuzzleOffset = 100.f;

	/** Whether the weapon can currently fire. */
	bool bCanFire = true;

	/** Timer handle for the cooldown. */
	FTimerHandle CooldownTimerHandle;

	/** Optional muzzle component.  If not set, the owner actor's root is used. */
	UPROPERTY(EditAnywhere, Category="Weapon|Muzzle")
	USceneComponent* MuzzleComponent;

	/** Whether to spawn a projectile in addition to the ray‑cast. */
	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	bool bSpawnProjectile = true;

	/** Whether to spawn a laser beam effect. */
	UPROPERTY(EditAnywhere, Category="Weapon|Laser")
	bool bSpawnLaserBeam = true;

	/** Whether to apply damage when the ray‑cast hits. */
	UPROPERTY(EditAnywhere, Category="Weapon|Damage")
	bool bApplyDamageOnHit = true;
};
