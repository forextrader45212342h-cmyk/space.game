// OrbitWeaponSystem.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponSystem.generated.h"

/**
 * Configuration struct for a weapon.
 * Can be edited in the editor or set at runtime.
 */
USTRUCT(BlueprintType)
struct FWeaponConfig
{
    GENERATED_BODY()

    /** Damage dealt by the weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 10.f;

    /** Maximum range for raycast weapons. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range = 1000.f;

    /** Time (seconds) between consecutive shots. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate = 0.2f;

    /** Projectile class to spawn when bSpawnProjectile is true. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AActor> ProjectileClass;

    /** Particle system used for laser visual effect. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* LaserEffect;

    /** Use a raycast to detect hits. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bUseRaycast = true;

    /** Spawn a projectile instead of using raycast. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bSpawnProjectile = false;
};

/**
 * Component that handles weapon firing, raycasting, projectile spawning,
 * damage application, and cooldown logic.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponSystem();

    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** Called every frame. Handles cooldown timing. */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Fire the weapon. Can be bound to input or AI. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void FireWeapon();

    /** Set the weapon configuration at runtime. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponConfig(const FWeaponConfig& NewConfig);

protected:
    /** Perform a raycast from the owner forward. Returns true if hit. */
    bool PerformRaycast(FHitResult& OutHit);

    /** Spawn a projectile actor at the specified location and rotation. */
    void SpawnProjectile(const FVector& SpawnLocation, const FRotator