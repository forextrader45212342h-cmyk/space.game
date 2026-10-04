// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  A reusable weapon component that handles:
 *  • Ray‑cast based laser firing
 *  • Projectile spawning (optional)
 *  • Cool‑down management
 *
 *  The component can be attached to any actor (e.g. a spaceship or a turret).
 *  It exposes a single public method `FireWeapon()` that will perform a
 *  line‑trace, spawn a projectile if configured, and apply a cooldown.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitWeaponSystem();

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Fire the weapon. Returns true if the weapon fired successfully. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireWeapon();

	/** Set the weapon's cooldown in seconds. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetCooldown(float NewCooldown);

	/** Set the projectile class to spawn. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetProjectileClass(TSubclassOf<AActor> NewProjectileClass);

	/** Set the damage value applied by the projectile. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetDamage(float NewDamage);

	/** Set the maximum range of the laser ray‑cast. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetRange(float NewRange);

	/** Set the socket name on the owning actor that represents the muzzle. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetMuzzleSocketName(FName NewSocketName);

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

private:
	/** Performs a line‑trace from the muzzle forward. */
	bool PerformRaycast(FHitResult& OutHit);

	/** Spawns a projectile at the muzzle location. */
	void SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation);

	/** Called when the cooldown timer expires. */
	void ResetCooldown();

	/** Whether the weapon is currently on cooldown. */
	bool bCanFire = true;

	/** Cool‑down duration in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float CooldownDuration = 0.5f;

	/** Damage dealt by the projectile. */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Damage = 25.f;

	/** Maximum range of the laser ray‑cast. */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Range = 10000.f;

	/** Projectile class to spawn (optional). */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

	/** Socket name on the owning actor that represents the muzzle. */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	/** Timer handle used for cooldown. */
	FTimerHandle CooldownTimerHandle;
};
