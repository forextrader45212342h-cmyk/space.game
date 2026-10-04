// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  OrbitWeaponSystem
 *
 *  Handles laser‑type weapon firing, ray‑casting for instant hit detection,
 *  projectile spawning for visual feedback, and a simple cooldown timer.
 *
 *  The component is intended to be attached to any Actor that can fire
 *  a weapon (e.g. a spaceship, turret, or character).
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitWeaponSystem();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Fire the weapon. Returns true if the shot was successful. */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool FireWeapon();

	/** Set the projectile class to spawn for visual feedback. */
	void SetProjectileClass(TSubclassOf<AActor> InProjectileClass);

	/** Set the damage value applied to hit actors. */
	void SetDamage(float InDamage);

	/** Set the maximum range of the laser. */
	void SetRange(float InRange);

	/** Set the cooldown between shots in seconds. */
	void SetCooldown(float InCooldown);

protected:
	/** Performs a raycast from the owner forward. */
	bool PerformRaycast(FHitResult& OutHit);

	/** Spawns a projectile at the muzzle location. */
	void SpawnProjectile(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);

	/** Called when the cooldown timer expires. */
	void ResetCooldown();

private:
	/** Projectile class used for visual feedback. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AActor> ProjectileClass;

	/** Damage applied to hit actors. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 25.f;

	/** Maximum range of the laser. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Range = 10000.f;

	/** Cooldown time between shots. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Cooldown = 0.5f;

	/** Whether the weapon is currently on cooldown. */
	bool bCanFire = true;

	/** Timer handle for the cooldown. */
	FTimerHandle CooldownTimerHandle;
};
