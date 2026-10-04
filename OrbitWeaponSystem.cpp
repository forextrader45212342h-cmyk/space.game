// OrbitWeaponSystem.cpp
//  UE5 C++ implementation of a laser weapon that performs a line‑trace,
//  applies damage, and records telemetry for each hit.

#include "OrbitWeaponSystem.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "OrbitDamageInterface.h"          // Custom interface for damageable actors
#include "OrbitTelemetrySubsystem.h"      // Custom subsystem for telemetry

// -----------------------------------------------------------------------------
//  UOrbitWeaponSystem
// -----------------------------------------------------------------------------

UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;   // Weapon system is event driven
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    // Cache the owner actor for convenience
    OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem has no owner!"));
    }

    // Find the muzzle component (if any)
    MuzzleComponent = OwnerActor->FindComponentByClass<USceneComponent>();
    if (!MuzzleComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem: No muzzle component found on %s"),
               *OwnerActor->GetName());
    }
}

void UOrbitWeaponSystem::FireLaser()
{
    if (!OwnerActor)
    {
        return;
    }

    // Determine start and end points of the laser
    FVector Start = MuzzleComponent ? MuzzleComponent->GetComponentLocation()
                                    : OwnerActor->GetActorLocation();
    FVector Forward = MuzzleComponent ? MuzzleComponent->GetForwardVector()
                                      : OwnerActor->GetActorForwardVector();
    FVector End = Start + Forward * MaxLaserRange;

    // Perform the line trace
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );

    // Visual debug
    DrawDebugLaser(Start, End, bHit, HitResult);

    if (bHit)
    {
        // Apply damage if the hit actor implements the damage interface
        ApplyDamage(HitResult);

        // Record telemetry
        RecordTelemetry(HitResult);
    }
}

void UOrbitWeaponSystem::ApplyDamage(const FHitResult& Hit)
{
    AActor* HitActor = Hit.GetActor();
    if (!HitActor)
    {
        return;
    }

    // Check for custom damage interface
    if (HitActor->GetClass()->ImplementsInterface(UOrbitDamageInterface::StaticClass()))
    {
        // Use the interface to apply damage
        IOrbitDamageInterface::Execute_ApplyDamage(
            HitActor,
            DamageAmount,
            OwnerActor,
            Hit.ImpactPoint,
            Hit.ImpactNormal
        );
    }
    else
    {
        // Fallback to the standard UE damage system
        UGameplayStatics::ApplyPointDamage(
            HitActor,
            DamageAmount,
            Hit.TraceStart - Hit.TraceEnd,   // Direction
            Hit,
            OwnerActor->GetInstigatorController(),
            OwnerActor,
            DamageType
        );
    }
}

void UOrbitWeaponSystem::RecordTelemetry(const FHitResult& Hit)
{
    // Retrieve the telemetry subsystem
    UOrbitTelemetrySubsystem* Telemetry = GetWorld()->GetSubsystem<UOrbitTelemetrySubsystem>();
    if (!Telemetry)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem: Telemetry subsystem not found."));
        return;
    }

    // Build a telemetry payload
    FOrbitTelemetryPayload Payload;
    Payload.HitActorName = Hit.GetActor() ? Hit.GetActor()->GetName() : TEXT("None");
    Payload.HitLocation = Hit.ImpactPoint;
    Payload.HitNormal = Hit.ImpactNormal;
    Payload.DamageDealt = DamageAmount;
    Payload.LaserRange = MaxLaserRange;
    Payload.Timestamp = FDateTime::UtcNow();

    // Send the payload
    Telemetry->RecordLaserHit(Payload);
}

void UOrbitWeaponSystem::DrawDebugLaser(const FVector& Start, const FVector& End, bool bHit, const FHitResult& Hit)
{
    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    float LifeTime = 0.1f; // Short lifetime for visual feedback

    DrawDebugLine(
        GetWorld(),
        Start,
        End,
        LineColor,
        false,
        LifeTime,
        0,
        2.0f
    );

    if (bHit)
    {
        DrawDebugSphere(
            GetWorld(),
            Hit.ImpactPoint,
            8.0f,
            12,
            FColor::Yellow,
            false,
            LifeTime
        );
    }
}
