// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  A lightweight weapon system that can fire laser‑style hitscan
 *  shots or spawn projectile actors.  It handles cooldowns,
 *  ray‑casting, and projectile spawning in a single component.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitWeaponSystem();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Fire the weapon.  Returns true if a shot was fired. */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool FireWeapon();

	/** Set the projectile class to spawn when not using hitscan. */
	void SetProjectileClass(TSubclassOf<AActor> InProjectileClass);

	/** Set the damage value for hitscan or projectile. */
	void SetDamage(float InDamage);

	/** Set the cooldown between shots (seconds). */
	void SetCooldown(float InCooldown);

	/** Enable or disable hitscan mode. */
	void SetHitscanEnabled(bool bEnabled);

protected:
	/** Perform a hitscan raycast and apply damage if hit. */
	void PerformHitscan();

	/** Spawn a projectile actor and launch it forward. */
	void SpawnProjectile();

	/** Helper to get the forward vector of the owner. */
	FVector GetFireDirection() const;

	/** Helper to get the world location from which to fire. */
	FVector GetFireLocation() const;

private:
	/** Projectile class to spawn when not using hitscan. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AActor> ProjectileClass;

	/** Damage dealt by the weapon. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 25.f;

	/** Cooldown between shots (seconds). */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Cooldown = 0.5f;

	/** Whether the weapon uses hitscan (laser) or projectile. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	bool bHitscanEnabled = true;

	/** Time remaining until the next shot can be fired. */
	float TimeUntilNextShot = 0.f;

	/** Collision channel used for hitscan. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TEnumAsByte<ECollisionChannel> HitscanChannel = ECC_Visibility;

	/** Maximum range of the hitscan ray. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float HitscanRange = 10000.f;

	/** Optional muzzle offset from the owner’s origin. */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector MuzzleOffset = FVector::ZeroVector;
};
