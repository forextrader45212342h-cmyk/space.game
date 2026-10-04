// OrbitWeaponSystem.cpp
// ---------------
//  Implements a simple laser weapon that performs a line‑trace, applies damage
//  and records telemetry data for each hit.
//
//  The implementation is intentionally lightweight so that it can be dropped
//  into any UE5 project that already has a component or actor that owns a
//  UOrbitWeaponComponent.  The component exposes a single public method
//  `FireLaser()` that can be called from a player controller, AI, or any
//  other gameplay code.
//
//  Telemetry is recorded via a custom UE_LOG category and a simple struct
//  that can be extended or sent to a remote analytics service if desired.

#include "OrbitWeaponSystem.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogOrbitWeapon, Log, All);

// -----------------------------------------------------------------------------
//  Telemetry struct
// -----------------------------------------------------------------------------
struct FWeaponTelemetry
{
    FVector ImpactPoint;
    FVector ImpactNormal;
    AActor* HitActor;
    float Damage;

    FWeaponTelemetry()
        : ImpactPoint(FVector::ZeroVector)
        , ImpactNormal(FVector::ZeroVector)
        , HitActor(nullptr)
        , Damage(0.f)
    {}
};

// -----------------------------------------------------------------------------
//  UOrbitWeaponComponent
// -----------------------------------------------------------------------------
UOrbitWeaponComponent::UOrbitWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default values – can be overridden in the editor or by the owning actor
    Damage = 25.f;
    TraceDistance = 10000.f;          // 10 000 units (~10 m)
    TraceChannel = ECC_Visibility;    // Use visibility channel for simplicity
    bDebugDraw = true;
}

void UOrbitWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    // Cache the owner actor for later use
    OwnerActor = GetOwner();
}

void UOrbitWeaponComponent::FireLaser()
{
    if (!OwnerActor)
    {
        UE_LOG(LogOrbitWeapon, Warning, TEXT("UOrbitWeaponComponent has no owner!"));
        return;
    }

    // 1. Determine start and end points of the trace
    FVector Start = GetComponentLocation();
    FVector Forward = GetComponentRotation().Vector();
    FVector End   = Start + Forward * TraceDistance;

    // 2. Setup query parameters
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);          // Don't hit ourselves
    QueryParams.bTraceComplex = true;                // Hit complex geometry
    QueryParams.bReturnPhysicalMaterial = false;

    // 3. Perform the line trace
    FHitResult HitResult;
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        TraceChannel,
        QueryParams
    );

    // 4. Debug draw the trace (optional)
    if (bDebugDraw)
    {
        FColor TraceColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            GetWorld(),
            Start,
            bHit ? HitResult.ImpactPoint : End,
            TraceColor,
            false,
            2.0f,
            0,
            1.0f
        );

        if (bHit)
        {
            DrawDebugSphere(
                GetWorld(),
                HitResult.ImpactPoint,
                8.f,
                12,
                FColor::Yellow,
                false,
                2.0f
            );
        }
    }

    // 5. If we hit something, apply damage and record telemetry
    if (bHit && HitResult.GetActor())
    {
        // Apply point damage – this will trigger any damage handlers on the hit actor
        UGameplayStatics::ApplyPointDamage(
            HitResult.GetActor(),
            Damage,
            Forward,
            HitResult,
            OwnerActor->GetInstigatorController(),
            this,
            DamageType
        );

        // Record telemetry
        FWeaponTelemetry Telemetry;
        Telemetry.ImpactPoint = HitResult.ImpactPoint;
        Telemetry.ImpactNormal = HitResult.ImpactNormal;
        Telemetry.HitActor = HitResult.GetActor();
        Telemetry.Damage = Damage;

        LogTelemetry(Telemetry);
    }
}

void UOrbitWeaponComponent::LogTelemetry(const FWeaponTelemetry& Telemetry)
{
    if (!Telemetry.HitActor)
    {
        UE_LOG(LogOrbitWeapon, Warning, TEXT("Telemetry: No actor hit."));
        return;
    }

    UE_LOG(
        LogOrbitWeapon,
        Log,
        TEXT("Laser hit %s at %s (normal %s) dealing %.1f damage."),
        *Telemetry.HitActor->GetName(),
        *Telemetry.ImpactPoint.ToString(),
        *Telemetry.ImpactNormal.ToString(),
        Telemetry.Damage
    );

    // TODO: Push telemetry to a remote analytics service if required.
    // Example:
    // AnalyticsSubsystem->RecordEvent(TEXT("LaserHit"), Telemetry);
}

// -----------------------------------------------------------------------------
//  OrbitWeaponSystem.h
// -----------------------------------------------------------------------------
/*
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponSystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ORBIT_API UOrbitWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponComponent();

    /** Fires the laser – performs a line trace, applies damage, and logs telemetry. */
    UFUNCTION(BlueprintCallable, Category="Weapon")
    void FireLaser();

protected:
    virtual void BeginPlay() override;

private:
    /** Applies damage to the hit actor and logs telemetry. */
    void LogTelemetry(const struct FWeaponTelemetry& Telemetry);

    /** Cached owner actor. */
    AActor* OwnerActor = nullptr;

    /** Damage dealt by the laser. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    float Damage = 25.f;

    /** Maximum trace distance. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    float TraceDistance = 10000.f;

    /** Collision channel used for the trace. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

    /** Whether to draw debug lines. */
    UPROPERTY(EditAnywhere, Category="Weapon")
    bool bDebugDraw = true;

    /** Damage type used when applying damage. */
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();
};
*/
