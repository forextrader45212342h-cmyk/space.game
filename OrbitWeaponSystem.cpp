// OrbitWeaponSystem.cpp
// Implements laser firing, line‑tracing, damage application and telemetry logging.

#include "OrbitWeaponSystem.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogOrbitWeapon, Log, All);

// -----------------------------------------------------------------------------
// Helper: Telemetry data structure
// -----------------------------------------------------------------------------
struct FLaserTelemetry
{
    FVector Start;
    FVector End;
    bool bHit;
    AActor* HitActor;
    float HitDistance;
    float DamageDealt;
    double Timestamp;

    FLaserTelemetry()
        : Start(FVector::ZeroVector)
        , End(FVector::ZeroVector)
        , bHit(false)
        , HitActor(nullptr)
        , HitDistance(0.f)
        , DamageDealt(0.f)
        , Timestamp(0.0)
    {}
};

// -----------------------------------------------------------------------------
// OrbitWeaponSystem implementation
// -----------------------------------------------------------------------------
UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
    Damage = 25.f;
    TraceRange = 10000.f;
    bDebugDraw = true;
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    // Optional: schedule periodic telemetry dump
    GetWorld()->GetTimerManager().SetTimer(
        TelemetryTimerHandle,
        this,
        &UOrbitWeaponSystem::DumpTelemetry,
        TelemetryInterval,
        true
    );
}

void UOrbitWeaponSystem::FireLaser()
{
    if (!GetOwner())
    {
        UE_LOG(LogOrbitWeapon, Warning, TEXT("OrbitWeaponSystem has no owner!"));
        return;
    }

    const FVector Start = GetOwner()->GetActorLocation();
    const FRotator Rotation = GetOwner()->GetActorRotation();
    const FVector Direction = Rotation.Vector();

    const FVector End = Start + Direction * TraceRange;

    FLaserTelemetry Telemetry;
    Telemetry.Start = Start;
    Telemetry.End = End;
    Telemetry.Timestamp = FPlatformTime::Seconds();

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());
    Params.bTraceComplex = true;
    Params.bReturnPhysicalMaterial = false;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    Telemetry.bHit = bHit;
    Telemetry.HitActor = bHit ? HitResult.GetActor() : nullptr;
    Telemetry.HitDistance = bHit ? HitResult.Distance : 0.f;

    if (bHit && Telemetry.HitActor)
    {
        // Apply damage
        const float AppliedDamage = UGameplayStatics::ApplyDamage(
            Telemetry.HitActor,
            Damage,
            GetOwner()->GetInstigatorController(),
            GetOwner(),
            UDamageType::StaticClass()
        );

        Telemetry.DamageDealt = AppliedDamage;

        UE_LOG(LogOrbitWeapon, Log,
            TEXT("Laser hit %s for %f damage (Distance: %f)"),
            *Telemetry.HitActor->GetName(),
            AppliedDamage,
            Telemetry.HitDistance
        );
    }
    else
    {
        UE_LOG(LogOrbitWeapon, Log,
            TEXT("Laser missed. Trace end: %s"),
            *End.ToString()
        );
    }

    // Store telemetry for later analysis
    TelemetryHistory.Add(Telemetry);

    // Optional debug drawing
    if (bDebugDraw)
    {
        const FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            LineColor,
            false,
            2.f,
            0,
            1.f
        );

        if (bHit)
        {
            DrawDebugPoint(
                GetWorld(),
                HitResult.ImpactPoint,
                10.f,
                FColor::Yellow,
                false,
                2.f
            );
        }
    }
}

void UOrbitWeaponSystem::DumpTelemetry()
{
    if (TelemetryHistory.Num() == 0)
    {
        UE_LOG(LogOrbitWeapon, Verbose, TEXT("No telemetry to dump."));
        return;
    }

    UE_LOG(LogOrbitWeapon, Log, TEXT("=== Laser Telemetry Dump (%d entries) ==="),
        TelemetryHistory.Num());

    for (const FLaserTelemetry& Entry : TelemetryHistory)
    {
        UE_LOG(LogOrbitWeapon, Log,
            TEXT("[%.3f] Start: %s | End: %s | Hit: %s | Actor: %s | Dist: %.1f | Damage: %.1f"),
            Entry.Timestamp,
            *Entry.Start.ToString(),
            *Entry.End.ToString(),
            Entry.bHit ? TEXT("Yes") : TEXT("No"),
            Entry.HitActor ? *Entry.HitActor->GetName() : TEXT("None"),
            Entry.HitDistance,
            Entry.DamageDealt
        );
    }

    TelemetryHistory.Empty();
}
