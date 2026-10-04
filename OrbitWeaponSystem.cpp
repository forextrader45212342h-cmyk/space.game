// OrbitWeaponSystem.cpp
// UE5 C++ implementation of a laser weapon system that performs line‑traces,
// applies damage, logs telemetry, and spawns impact effects.

#include "OrbitWeaponSystem.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "GameFramework/DamageType.h"
#include "Components/SceneComponent.h"

//////////////////////////////////////////////////////////////////////////
// Telemetry

struct FLaserTelemetry
{
    FVector Start;
    FVector End;
    bool bHit;
    float DamageDealt;
    float HitDistance;
    FString HitActorName;

    FLaserTelemetry()
        : Start(FVector::ZeroVector)
        , End(FVector::ZeroVector)
        , bHit(false)
        , DamageDealt(0.f)
        , HitDistance(0.f)
        , HitActorName(TEXT(""))
    {}
};

static void LogLaserTelemetry(const FLaserTelemetry& Telemetry)
{
    UE_LOG(LogTemp, Log,
        TEXT("[LaserTelemetry] Start: %s, End: %s, Hit: %s, Damage: %.2f, Distance: %.2f, Actor: %s"),
        *Telemetry.Start.ToString(),
        *Telemetry.End.ToString(),
        Telemetry.bHit ? TEXT("Yes") : TEXT("No"),
        Telemetry.DamageDealt,
        Telemetry.HitDistance,
        *Telemetry.HitActorName);
}

//////////////////////////////////////////////////////////////////////////
// AOrbitWeaponSystem

AOrbitWeaponSystem::AOrbitWeaponSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Default values
    LaserRange = 10000.f;
    DamageAmount = 25.f;
    FireRate = 0.2f; // 5 shots per second
    bContinuousFire = false;
    bIsFiring = false;
}

void AOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    if (bContinuousFire)
    {
        StartContinuousFire();
    }
}

void AOrbitWeaponSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AOrbitWeaponSystem::StartContinuousFire()
{
    if (!bIsFiring)
    {
        bIsFiring = true;
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AOrbitWeaponSystem::FireLaser, FireRate, true, 0.f);
    }
}

void AOrbitWeaponSystem::StopContinuousFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }
}

void AOrbitWeaponSystem::FireLaser()
{
    // 1. Perform line trace
    FHitResult HitResult;
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * LaserRange);

    bool bHit = PerformLineTrace(Start, End, HitResult);

    // 2. Apply damage if hit
    float DamageDealt = 0.f;
    if (bHit)
    {
        DamageDealt = ApplyDamage(HitResult);
    }

    // 3. Spawn impact effect
    SpawnImpactEffect(bHit ? HitResult.ImpactPoint : End);

    // 4. Log telemetry
    FLaserTelemetry Telemetry;
    Telemetry.Start = Start;
    Telemetry.End = End;
    Telemetry.bHit = bHit;
    Telemetry.DamageDealt = DamageDealt;
    Telemetry.HitDistance = bHit ? HitResult.Distance : LaserRange;
    Telemetry.HitActorName = bHit ? HitResult.GetActor()->GetName() : TEXT("None");
    LogLaserTelemetry(Telemetry);

    // 5. Debug drawing
    DrawDebugLaser(Start, End, bHit, HitResult);
}

bool AOrbitWeaponSystem::PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHit)
{
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    if (GetOwner())
    {
        QueryParams.AddIgnoredActor(GetOwner());
    }
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        OutHit,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );

    return bHit;
}

float AOrbitWeaponSystem::ApplyDamage(const FHitResult& Hit)
{
    if (!Hit.GetActor())
    {
        return 0.f;
    }

    // Use point damage for simplicity
    UGameplayStatics::ApplyPointDamage(
        Hit.GetActor(),
        DamageAmount,
        Hit.TraceStart - Hit.TraceEnd, // Direction
        Hit,
        GetInstigatorController(),
        this,
        DamageTypeClass
    );

    return DamageAmount;
}

void AOrbitWeaponSystem::SpawnImpactEffect(const FVector& ImpactLocation)
{
    if (!ImpactEffect)
    {
        return;
    }

    UGameplayStatics::SpawnEmitterAtLocation(
        GetWorld(),
        ImpactEffect,
        ImpactLocation,
        FRotator::ZeroRotator,
        true
    );
}

void AOrbitWeaponSystem::DrawDebugLaser(const FVector& Start, const FVector& End, bool bHit, const FHitResult& Hit)
{
#if ENABLE_DRAW_DEBUG
    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.1f, 0, 1.f);

    if (bHit)
    {
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 12, FColor::Yellow, false, 0.1f);
    }
#endif
}
