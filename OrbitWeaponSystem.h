// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  UOrbitWeaponSystem
 *
 *  A reusable component that implements a laser‑style weapon.
 *  It performs a raycast to detect hits, spawns a projectile
 *  (if a ProjectileClass is set), applies damage, and enforces
 *  a cooldown between shots.
 *
 *  The component is Blueprint‑spawnable and can be attached to
 *  any actor that owns a skeletal mesh with a muzzle socket.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitWeaponSystem();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/**
	 *  Fires the weapon.
	 *
	 *  @return true if the weapon was fired, false if still on cooldown.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireWeapon();

protected:
	/** Time between consecutive shots (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Cooldown")
	float CooldownTime = 0.5f;

	/** Timestamp of the last shot (in world time). */
	float LastFireTime = -INFINITY;

	/** Projectile class to spawn when the weapon fires. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	/** Socket name on the owning mesh that represents the muzzle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Muzzle")
	FName MuzzleSocketName = FName(TEXT("Muzzle"));

	/** Damage applied to the first hit actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
	float Damage = 25.f;

	/** Collision channel used for the laser raycast. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** Optional material used to draw a debug laser. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
	UMaterialInterface* LaserMaterial = nullptr;

	/** Cached reference to the owning actor's mesh component. */
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh = nullptr;

	/** Performs a raycast from the muzzle forward. */
	bool PerformRaycast(FHitResult& OutHit) const;

	/** Spawns a projectile at the muzzle. */
	void SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;

	/** Draws a debug laser line. */
	void DrawLaser(const FVector& Start, const FVector& End) const;
};

// ---------------------------------------------------------------------------
// Inline implementations
// ---------------------------------------------------------------------------

inline UOrbitWeaponSystem::UOrbitWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

inline void UOrbitWeaponSystem::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owner's mesh component if it exists
	if (AActor* Owner = GetOwner())
	{
		OwnerMesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	}
}

inline bool UOrbitWeaponSystem::FireWeapon()
{
	// Check cooldown
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < CooldownTime)
	{
		return false; // Still cooling down
	}

	// Update last fire time
	LastFireTime = CurrentTime;

	// Get muzzle world location and rotation
	FVector MuzzleLocation = FVector::ZeroVector;
	FRotator MuzzleRotation = FRotator::ZeroRotator;

	if (OwnerMesh && OwnerMesh->DoesSocketExist(MuzzleSocketName))
	{
		MuzzleLocation = OwnerMesh->GetSocketLocation(MuzzleSocketName);
		MuzzleRotation = OwnerMesh->GetSocketRotation(MuzzleSocketName);
	}
	else if (AActor* Owner = GetOwner())
	{
		// Fallback to actor's transform
		MuzzleLocation = Owner->GetActorLocation();
		MuzzleRotation = Owner->GetActorRotation();
	}

	// Perform raycast
	FHitResult HitResult;
	if (PerformRaycast(HitResult))
	{
		// Apply damage to the hit actor
		if (AActor* HitActor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				Damage,
				MuzzleRotation.Vector(),
				HitResult,
				GetOwner()->GetInstigatorController(),
				GetOwner(),
				nullptr // DamageType can be set if needed
			);
		}
	}

	// Spawn projectile if a class is set
	if (ProjectileClass)
	{
		SpawnProjectile(MuzzleLocation, MuzzleRotation);
	}

	// Draw debug laser
	DrawLaser(MuzzleLocation, HitResult.bBlockingHit ? HitResult.ImpactPoint : MuzzleLocation + MuzzleRotation.Vector() * 10000.f);

	return true;
}

inline bool UOrbitWeaponSystem::PerformRaycast(FHitResult& OutHit) const
{
	if (!GetWorld() || !OwnerMesh)
	{
		return false;
	}

	FVector Start = OwnerMesh->GetSocketLocation(MuzzleSocketName);
	FVector End = Start + OwnerMesh->GetSocketRotation(MuzzleSocketName).Vector() * 10000.f; // 10k units

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		TraceChannel,
		Params
	);
}

inline void UOrbitWeaponSystem::SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if (!GetWorld() || !ProjectileClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

inline void UOrbitWeaponSystem::DrawLaser(const FVector& Start, const FVector& End) const
{
	if (!GetWorld())
	{
		return;
	}

	FColor LaserColor = FColor::Red;
	if (LaserMaterial)
	{
		// If a material is provided, use it to draw a dynamic line
		DrawDebugLine(GetWorld(), Start, End, LaserColor, false, 0.1f, 0, 2.f);
	}
	else
	{
		// Default debug line
		DrawDebugLine(GetWorld(), Start, End, LaserColor, false, 0.1f, 0, 2.f);
	}
}
