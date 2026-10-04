// OrbitWeaponSystem.cpp
// ---------------
// A minimal UE5 C++ implementation of a laser‑weapon system that
// performs a line‑trace, applies damage to the hit actor and
// records telemetry for each shot.
//
// The code is intentionally lightweight – it can be dropped into a
// UE5 project and compiled without any additional dependencies.
//
// 1.  The weapon is implemented as an Actor component so it can be
//     attached to any pawn or character.
// 2.  `FireLaser()` performs a single line‑trace using the
//     `ECC_Visibility` channel.
// 3.  If an actor is hit, `UGameplayStatics::ApplyDamage` is called
//     with a configurable damage amount.
// 4.  Telemetry data (hit location, hit actor, hit distance,
//     whether the shot hit something) is logged via `UE_LOG` and
//     can be extended to send to an analytics backend.
//
// -----------------------------------------------------------------

#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#endif

// ------------------------------------------------------------------
// Helper struct for telemetry – can be expanded or replaced with
// a proper analytics SDK.
// ------------------------------------------------------------------
struct FLaserTelemetry
{
    FVector Start;
    FVector End;
    AActor* HitActor;
    float HitDistance;
    bool bHit;

    FString ToString() const
    {
        return FString::Printf(TEXT(
            "LaserShot: Start=%s End=%s Hit=%s Distance=%.2f"),
            *Start.ToString(),
            *End.ToString(),
            bHit ? *HitActor->GetName() : TEXT("None"),
            HitDistance);
    }
};

// ------------------------------------------------------------------
// UOrbitWeaponSystem
// ------------------------------------------------------------------
UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    // Default values – can be overridden in the editor
    Damage = 25.f;
    MaxRange = 10000.f; // 100 meters
    bDebugDraw = true;
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    // Cache the owning actor for later use
    OwnerActor = GetOwner();
}

void UOrbitWeaponSystem::FireLaser()
{
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem: No owner actor."));
        return;
    }

    // 1.  Determine start and end points
    const FVector Start = GetMuzzleLocation();
    const FVector Forward = OwnerActor->GetActorForwardVector();
    const FVector End = Start + Forward * MaxRange;

    // 2.  Perform line trace
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerActor);
    Params.bTraceComplex = true;
    Params.bReturnPhysicalMaterial = false;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        Params);

    // 3.  Apply damage if we hit something
    if (bHit && HitResult.GetActor())
    {
        UGameplayStatics::ApplyDamage(
            HitResult.GetActor(),
            Damage,
            OwnerActor->GetInstigatorController(),
            OwnerActor,
            UDamageType::StaticClass());

        // Optional: spawn a hit effect, play sound, etc.
    }

    // 4.  Telemetry
    FLaserTelemetry Telemetry;
    Telemetry.Start = Start;
    Telemetry.End = End;
    Telemetry.bHit = bHit;
    Telemetry.HitActor = bHit ? HitResult.GetActor() : nullptr;
    Telemetry.HitDistance = bHit ? HitResult.Distance : MaxRange;

    UE_LOG(LogTemp, Log, TEXT("%s"), *Telemetry.ToString());

    // 5.  Debug drawing
    if (bDebugDraw)
    {
        const FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            LineColor,
            false,
            2.0f,
            0,
            2.0f);

        if (bHit)
        {
            DrawDebugSphere(
                GetWorld(),
                HitResult.ImpactPoint,
                8.f,
                12,
                FColor::Yellow,
                false,
                2.0f);
        }
    }
}

// ------------------------------------------------------------------
// Helper to get the muzzle location – by default we use the
// component's world location.  Override this if you want a
// specific socket or child component.
// ------------------------------------------------------------------
FVector UOrbitWeaponSystem::GetMuzzleLocation() const
{
    // If the component has a socket named "Muzzle", use it.
    if (const USkeletalMeshComponent* Skel = Cast<USkeletalMeshComponent>(OwnerActor->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
    {
        if (Skel->DoesSocketExist(TEXT("Muzzle")))
        {
            return Skel->GetSocketLocation(TEXT("Muzzle"));
        }
    }

    // Fallback to the component's world location
    return GetComponentLocation();
}

// ------------------------------------------------------------------
// Optional: expose a Blueprint callable wrapper
// ------------------------------------------------------------------
#if WITH_EDITOR
void UOrbitWeaponSystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    // Clamp values to sane ranges
    Damage = FMath::Clamp(Damage, 0.f, 1000.f);
    MaxRange = FMath::Clamp(MaxRange, 100.f, 20000.f);
}
#endif

// ------------------------------------------------------------------
// OrbitWeaponSystem.h
// ------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponSystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponSystem();

    /** Fires a laser shot – performs a line trace, applies damage and logs telemetry. */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    void FireLaser();

protected:
    virtual void BeginPlay() override;

private:
    /** Returns the world location of the muzzle. Override if you have a socket. */
    FVector GetMuzzleLocation() const;

    /** The actor that owns this component. Cached for performance. */
    AActor* OwnerActor = nullptr;

    /** Damage applied to a hit actor. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    float Damage;

    /** Maximum range of the laser in centimeters. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    float MaxRange;

    /** Draw debug lines and hit spheres. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    bool bDebugDraw;
};
