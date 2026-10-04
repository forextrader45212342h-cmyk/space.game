// OrbitWeaponSystem.cpp
// Implements laser firing, line‑trace damage, and telemetry logging.

#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OrbitTelemetry.h"          // Custom telemetry header
#include "OrbitDamageTypes.h"        // Custom damage type header

// -----------------------------------------------------------------------------
// Telemetry helper
// -----------------------------------------------------------------------------
static void LogLaserTelemetry(const FLaserTelemetry& Telemetry)
{
    // Simple console log – replace with your telemetry sink if needed
    UE_LOG(LogOrbit, Log,
        TEXT("Laser hit: %s | Location: %s | Damage: %.1f | Normal: %s"),
        *Telemetry.HitActorName,
        *Telemetry.HitLocation.ToString(),
        Telemetry.Damage,
        *Telemetry.HitNormal.ToString());
}

// -----------------------------------------------------------------------------
// Firing logic
// -----------------------------------------------------------------------------
void UOrbitWeaponSystem::FireLaser()
{
    if (!Owner || !Owner->GetWorld())
    {
        return;
    }

    // 1. Get muzzle world transform
    const FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();
    const FRotator MuzzleRotation = MuzzleComponent->GetComponentRotation();

    // 2. Compute end point
    const FVector End = MuzzleLocation + MuzzleRotation.Vector() * LaserRange;

    // 3. Setup query params
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;

    // 4. Perform line trace
    FHitResult HitResult;
    const bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        HitResult,
        MuzzleLocation,
        End,
        ECC_Visibility,
        QueryParams
    );

    // 5. Draw debug line (optional)
    DrawDebugLine(
        Owner->GetWorld(),
        MuzzleLocation,
        bHit ? HitResult.ImpactPoint : End,
        bHit ? FColor::Red : FColor::Green,
        false,
        2.0f,
        0,
        1.0f
    );

    // 6. Telemetry data
    FLaserTelemetry Telemetry;
    Telemetry.HitActorName = bHit && HitResult.GetActor()
        ? HitResult.GetActor()->GetName()
        : TEXT("None");
    Telemetry.HitLocation = bHit ? HitResult.ImpactPoint : End;
    Telemetry.HitNormal = bHit ? HitResult.ImpactNormal : FVector::ZeroVector;
    Telemetry.Damage = 0.0f;

    // 7. Apply damage if hit
    if (bHit && HitResult.GetActor())
    {
        const float DamageAmount = BaseDamage;

        // Apply damage using UE's damage system
        UGameplayStatics::ApplyDamage(
            HitResult.GetActor(),
            DamageAmount,
            Owner->GetInstigatorController(),
            this,
            UOrbitDamageType::StaticClass()
        );

        Telemetry.Damage = DamageAmount;
    }

    // 8. Log telemetry
    LogLaserTelemetry(Telemetry);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default values
    BaseDamage = 25.0f;
    LaserRange = 10000.0f; // 100 meters

    // Create a default muzzle component
    MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
    MuzzleComponent->SetupAttachment(this);
}

// -----------------------------------------------------------------------------
// Called when the game starts
// -----------------------------------------------------------------------------
void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner();
}

// -----------------------------------------------------------------------------
// Public API to trigger a laser shot
// -----------------------------------------------------------------------------
void UOrbitWeaponSystem::TriggerLaser()
{
    FireLaser();
}
