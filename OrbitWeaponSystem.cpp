// OrbitWeaponSystem.cpp
// Implements a laser weapon that performs a line‑trace, applies damage,
// and records telemetry data for later analysis.

#include "OrbitWeaponSystem.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// UOrbitWeaponSystem

UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    // Enable ticking if you want to flush telemetry periodically
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    // Optional: start a timer to flush telemetry every few seconds
    GetWorld()->GetTimerManager().SetTimer(
        TelemetryFlushTimer,
        this,
        &UOrbitWeaponSystem::FlushTelemetry,
        TelemetryFlushInterval,
        true
    );
}

//////////////////////////////////////////////////////////////////////////
// Public API

/**
 * Fires a laser from the given start point in the given direction.
 *
 * @param Start          World space start location of the laser.
 * @param Direction      Normalized direction vector.
 * @param Range          Maximum trace distance.
 * @param Damage         Damage to apply on hit.
 */
void UOrbitWeaponSystem::FireLaser(
    const FVector& Start,
    const FVector& Direction,
    float Range,
    float Damage)
{
    if (!GetWorld())
    {
        return;
    }

    // Draw a debug line for visual feedback
    const FVector End = Start + Direction * Range;
    DrawDebugLine(
        GetWorld(),
        Start,
        End,
        FColor::Red,
        false,
        1.0f,
        0,
        1.0f
    );

    // Perform the line trace
    FHitResult HitResult;
    if (PerformLineTrace(Start, End, HitResult))
    {
        // Apply damage to the hit actor
        ApplyDamage(HitResult, Damage);

        // Record telemetry
        RecordTelemetry(HitResult, Damage);
    }
}

//////////////////////////////////////////////////////////////////////////
// Internal helpers

/**
 * Performs a single channel line trace.
 *
 * @param Start      Trace start location.
 * @param End        Trace end location.
 * @param HitResult  Out hit result.
 * @return           true if something was hit.
 */
bool UOrbitWeaponSystem::PerformLineTrace(
    const FVector& Start,
    const FVector& End,
    FHitResult& HitResult) const
{
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    QueryParams.AddIgnoredActor(GetOwner());

    return GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );
}

/**
 * Applies point damage to the hit actor.
 *
 * @param HitResult  Result from the line trace.
 * @param Damage     Amount of damage to apply.
 */
void UOrbitWeaponSystem::ApplyDamage(
    const FHitResult& HitResult,
    float Damage) const
{
    if (!HitResult.GetActor())
    {
        return;
    }

    UGameplayStatics::ApplyPointDamage(
        HitResult.GetActor(),
        Damage,
        HitResult.TraceStart - HitResult.TraceEnd, // direction
        HitResult,
        GetOwner()->GetInstigatorController(),
        GetOwner(),
        DamageType
    );
}

/**
 * Stores telemetry data for later analysis.
 *
 * @param HitResult  Result from the line trace.
 * @param Damage     Damage that was applied.
 */
void UOrbitWeaponSystem::RecordTelemetry(
    const FHitResult& HitResult,
    float Damage)
{
    FWeaponTelemetry Telemetry;
    Telemetry.HitLocation = HitResult.ImpactPoint;
    Telemetry.HitActor = HitResult.GetActor();
    Telemetry.DamageDealt = Damage;
    Telemetry.Timestamp = GetWorld()->GetTimeSeconds();

    TelemetryLog.Add(Telemetry);
}

/**
 * Flushes the telemetry buffer to disk or a network endpoint.
 * This is a placeholder – replace with your own persistence logic.
 */
void UOrbitWeaponSystem::FlushTelemetry()
{
    if (TelemetryLog.Num() == 0)
    {
        return;
    }

    // Example: log to the console
    for (const FWeaponTelemetry& Telemetry : TelemetryLog)
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("Laser hit %s at %s, damage: %.2f"),
            Telemetry.HitActor ? *Telemetry.HitActor->GetName() : TEXT("None"),
            *Telemetry.HitLocation.ToString(),
            Telemetry.DamageDealt
        );
    }

    TelemetryLog.Empty();
}

//////////////////////////////////////////////////////////////////////////
// Telemetry struct definition (usually in the header)

#if WITH_EDITOR
// Only compile this in editor builds to keep shipping builds lean.
void UOrbitWeaponSystem::PrintTelemetry() const
{
    for (const FWeaponTelemetry& Telemetry : TelemetryLog)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Telemetry: Actor=%s, Location=%s, Damage=%.2f, Time=%.2f"),
            Telemetry.HitActor ? *Telemetry.HitActor->GetName() : TEXT("None"),
            *Telemetry.HitLocation.ToString(),
            Telemetry.DamageDealt,
            Telemetry.Timestamp
        );
    }
}
#endif
