// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  UOrbitWeaponSystemComponent
 *
 *  A lightweight weapon system that:
 *  - Performs a raycast (laser) to detect hits.
 *  - Spawns a projectile actor at the muzzle.
 *  - Enforces a cooldown between shots.
 *
 *  The component is intended to be attached to any Actor that
 *  should be able to fire a weapon (e.g. a spaceship, a turret, etc.).
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitWeaponSystemComponent();

	/** Fire the weapon.  Returns true if a shot was fired. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireWeapon();

	/** Set the projectile class to spawn. */
	void SetProjectileClass(TSubclassOf<AActor> InClass) { ProjectileClass = InClass; }

	/** Set the muzzle socket name. */
	void SetMuzzleSocketName(FName InSocketName) { MuzzleSocketName = InSocketName; }

	/** Set the cooldown time in seconds. */
	void SetCooldown(float InCooldown) { CooldownTime = InCooldown; }

protected:
	virtual void BeginPlay() override;

private:
	/** Performs the raycast and returns the hit result. */
	bool PerformRaycast(FHitResult& OutHit);

	/** Spawns the projectile at the muzzle. */
	void SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation);

	/** Resets the ability to fire after cooldown. */
	void ResetFire();

private:
	/** Class of the projectile to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

	/** Name of the socket on the owning actor that represents the muzzle. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName = FName(TEXT("Muzzle"));

	/** Distance of the laser raycast. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RaycastDistance = 10000.0f;

	/** Time in seconds between shots. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float CooldownTime = 0.5f;

	/** Whether the weapon can currently fire. */
	bool bCanFire = true;
};
