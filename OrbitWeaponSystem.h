// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "OrbitWeaponSystem.generated.h"

/**
 *  UOrbitWeaponSystem
 *  ------------------
 *  A reusable component that implements a laser‑style weapon.
 *  • Performs a raycast from the owning actor's muzzle.
 *  • Spawns a projectile (if a projectile class is set) or draws a debug laser.
 *  • Enforces a cooldown between shots.
 *
 *  The component is intentionally lightweight so it can be dropped onto any
 *  actor that needs a simple laser weapon.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitWeaponSystem();

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Fire the weapon. Returns true if a shot was fired. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireWeapon();

	/** Set the muzzle socket name (used for raycast origin). */
	void SetMuzzleSocketName(FName NewSocketName) { MuzzleSocketName = NewSocketName; }

	/** Set the projectile class to spawn. If null, a debug laser is drawn. */
	void SetProjectileClass(TSubclassOf<AActor> NewClass) { ProjectileClass = NewClass; }

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

private:
	/** Performs the raycast and returns the hit result. */
	bool PerformRaycast(FHitResult& OutHit) const;

	/** Spawns a projectile at the muzzle location. */
	void SpawnProjectile(const FHitResult& Hit);

	/** Draws a debug laser line. */
	void DrawDebugLaser(const FHitResult& Hit) const;

	/** Returns true if the weapon is off cooldown. */
	bool IsOffCooldown() const;

	/** Records the time of the last shot. */
	void UpdateLastFireTime();

private:
	/** Name of the socket used as the muzzle. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** Class of the projectile to spawn. If null, a debug laser is drawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass = nullptr;

	/** Maximum range of the laser/raycast. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxRange = 10000.f;

	/** Cooldown time between shots in seconds. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float CooldownTime = 0.5f;

	/** Time of the last shot (in world time). */
	float LastFireTime = -FLT_MAX;

	/** Whether the component should tick. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bTickEnabled = false;
};

// OrbitWeaponSystem.cpp
#include "OrbitWeaponSystem.h"

UOrbitWeaponSystem::UOrbitWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = bTickEnabled;
}

void UOrbitWeaponSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UOrbitWeaponSystem::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// No per‑frame logic needed for this component.
}

bool UOrbitWeaponSystem::FireWeapon()
{
	if (!IsOffCooldown())
	{
		return false;
	}

	FHitResult Hit;
	if (!PerformRaycast(Hit))
	{
		// Nothing hit – still count as a shot.
		UpdateLastFireTime();
		return true;
	}

	if (ProjectileClass)
	{
		SpawnProjectile(Hit);
	}
	else
	{
		DrawDebugLaser(Hit);
	}

	UpdateLastFireTime();
	return true;
}

bool UOrbitWeaponSystem::PerformRaycast(FHitResult& OutHit) const
{
	AActor* Owner = GetOwner();
	if (!Owner) return false;

	FVector MuzzleLocation = Owner->GetActorLocation();
	FVector ForwardVector = Owner->GetActorForwardVector();

	// If a socket is defined, use it as the origin.
	if (Owner->GetRootComponent() && MuzzleSocketName != NAME_None)
	{
		MuzzleLocation = Owner->GetRootComponent()->GetSocketLocation(MuzzleSocketName);
	}

	FVector End = MuzzleLocation + ForwardVector * MaxRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	QueryParams.bTraceComplex = true;

	return GetWorld()->LineTraceSingleByChannel(
		OutHit,
		MuzzleLocation,
		End,
		ECC_Visibility,
		QueryParams);
}

void UOrbitWeaponSystem::SpawnProjectile(const FHitResult& Hit)
{
	if (!ProjectileClass) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector MuzzleLocation = Owner->GetActorLocation();
	FRotator MuzzleRotation = Owner->GetActorRotation();

	if (Owner->GetRootComponent() && MuzzleSocketName != NAME_None)
	{
		MuzzleLocation = Owner->GetRootComponent()->GetSocketLocation(MuzzleSocketName);
		MuzzleRotation = Owner->GetRootComponent()->GetSocketRotation(MuzzleSocketName);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(
		ProjectileClass,
		MuzzleLocation,
		MuzzleRotation,
		SpawnParams);

	if (!Projectile) return;

	// If the projectile has a movement component, set its velocity.
	if (UProjectileMovementComponent* MoveComp = Projectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		MoveComp->Velocity = MuzzleRotation.Vector() * MoveComp->InitialSpeed;
	}
}

void UOrbitWeaponSystem::DrawDebugLaser(const FHitResult& Hit) const
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector MuzzleLocation = Owner->GetActorLocation();
	if (Owner->GetRootComponent() && MuzzleSocketName != NAME_None)
	{
		MuzzleLocation = Owner->GetRootComponent()->GetSocketLocation(MuzzleSocketName);
	}

	FVector End = Hit.bBlockingHit ? Hit.ImpactPoint : MuzzleLocation + Owner->GetActorForwardVector() * MaxRange;

	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		End,
		FColor::Red,
		false,
		0.1f,
		0,
		2.f);
}

bool UOrbitWeaponSystem::IsOffCooldown() const
{
	return (GetWorld()->GetTimeSeconds() - LastFireTime) >= CooldownTime;
}

void UOrbitWeaponSystem::UpdateLastFireTime()
{
	LastFireTime = GetWorld()->GetTimeSeconds();
}
