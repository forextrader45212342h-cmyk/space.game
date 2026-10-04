// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  OrbitWeaponSystem
 *  -----------------
 *  Handles laser‑style weapon firing with ray‑casting, projectile spawning,
 *  and a configurable cooldown.  Designed to be attached to any Actor that
 *  should be able to fire a weapon (e.g. a spaceship, turret, or player).
 *
 *  Features
 *  --------
 *  • Ray‑cast to detect hit targets immediately.
 *  • Optional projectile spawn for visual/physics feedback.
 *  • Cooldown timer to limit fire rate.
 *  • Damage, range, and projectile properties are exposed to Blueprints.
 *
 *  Usage
 *  -----
 *  1. Add this component to an Actor.
 *  2. Bind the FireWeapon() function to an input action or call it manually.
 *  3. Override OnHitTarget() in a subclass if you want custom hit logic.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Constructor */
	UOrbitWeaponSystem();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Fire the weapon.  Returns true if a shot was fired. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireWeapon();

	/** Called when the weapon is successfully fired (after cooldown). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponFired();

	/** Called when a target is hit by the raycast. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnHitTarget(AActor* HitActor, const FVector& HitLocation, const FVector& HitNormal);

protected:
	/** Perform a raycast from the weapon's muzzle.  Returns true if something was hit. */
	bool PerformRaycast(FHitResult& OutHit);

	/** Spawn a projectile at the muzzle location. */
	void SpawnProjectile(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);

	/** Reset the cooldown timer. */
	void ResetCooldown();

	/** Called when the cooldown timer expires. */
	void OnCooldownComplete();

	/** Helper to get the world context. */
	UWorld* GetWorldContext() const;

	/** Muzzle socket name (used for projectile spawn and raycast origin). */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** Damage dealt by the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 25.f;

	/** Maximum range of the weapon (used for raycast). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 10000.f;

	/** Fire rate in shots per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 2.f; // 2 shots per second

	/** Projectile class to spawn (optional). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

	/** Projectile speed (if a projectile is spawned). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ProjectileSpeed = 3000.f;

	/** Whether the weapon should spawn a projectile in addition to the raycast. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bSpawnProjectile = true;

	/** Internal timer handle for cooldown. */
	FTimerHandle CooldownTimerHandle;

	/** Flag indicating whether the weapon is currently cooling down. */
	bool bIsCoolingDown = false;
};
