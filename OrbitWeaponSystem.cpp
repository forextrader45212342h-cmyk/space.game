// OrbitWeaponSystem.cpp
// ---------------
// UE5 C++ implementation of a simple laser weapon system that performs a
// line‑trace, applies damage, spawns an impact effect and logs telemetry.
//
// The component is intended to be attached to any actor that owns a
// "muzzle" socket or transform.  It exposes a single public method
// `FireLaser()` that can be called from an input binding or AI logic.
//
// Note:  The header file (OrbitWeaponSystem.h) is assumed to contain the
// minimal declarations shown below.  Only the .cpp file is shown here
// because the task explicitly requested the implementation file.
//
// -------------------------------------------------------------------------

#include "OrbitWeaponSystem.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"

#define LOCTEXT_NAMESPACE "OrbitWeaponSystem"

//////////////////////////////////////////////////////////////////////////
// UOrbitWeaponSystem

UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default values – can be overridden in the editor
    MaxRange          = 10000.f;          // 10,000 units
    DamageAmount      = 25.f;
    bUseDebugLine     = true;
    ImpactEffect      = nullptr;
    MuzzleSocketName  = TEXT("Muzzle");
}

void UOrbitWeaponSystem::BeginPlay()
{
    Super::BeginPlay();

    // Cache the owning actor for convenience
    OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem attached to null actor!"));
    }
}

//////////////////////////////////////////////////////////////////////////
// Public API

void UOrbitWeaponSystem::FireLaser()
{
    if (!OwnerActor)
    {
        return;
    }

    // 1. Determine start and end points
    FVector StartLocation = GetMuzzleLocation();
    FVector ForwardVector = OwnerActor->GetActorForwardVector();
    FVector EndLocation   = StartLocation + (ForwardVector * MaxRange);

    // 2. Perform line trace
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );

    // 3. Telemetry – log the trace
    UE_LOG(LogTemp, Log, TEXT("[OrbitWeapon] Laser fired from %s to %s, hit: %s"),
        *StartLocation.ToString(),
        *EndLocation.ToString(),
        bHit ? *HitResult.GetActor()->GetName() : TEXT("None")
    );

    // 4. Visual debug line (optional)
    if (bUseDebugLine)
    {
        FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            GetWorld(),
            StartLocation,
            bHit ? HitResult.Location : EndLocation,
            LineColor,
            false,
            2.0f,
            0,
            1.0f
        );
    }

    // 5. If we hit something, apply damage and spawn impact effect
    if (bHit)
    {
        // Apply point damage – this will trigger any damage handlers on the hit actor
        UGameplayStatics::ApplyPointDamage(
            HitResult.GetActor(),
            DamageAmount,
            ForwardVector,
            HitResult,
            OwnerActor->GetInstigatorController(),
            OwnerActor,
            DamageTypeClass
        );

        // Telemetry – damage applied
        UE_LOG(LogTemp, Log, TEXT("[OrbitWeapon] Applied %f damage to %s"),
            DamageAmount,
            *HitResult.GetActor()->GetName()
        );

        // Spawn impact effect at hit location
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                ImpactEffect,
                HitResult.Location,
                HitResult.ImpactNormal.Rotation(),
                true
            );
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Helper functions

FVector UOrbitWeaponSystem::GetMuzzleLocation() const
{
    // Try to find a socket on the owner's root component
    if (OwnerActor)
    {
        USceneComponent* RootComp = OwnerActor->GetRootComponent();
        if (RootComp && RootComp->DoesSocketExist(MuzzleSocketName))
        {
            return RootComp->GetSocketLocation(MuzzleSocketName);
        }
    }

    // Fallback to actor location
    return OwnerActor ? OwnerActor->GetActorLocation() : FVector::ZeroVector;
}

//////////////////////////////////////////////////////////////////////////
// Telemetry helpers (could be expanded to send data to an analytics system)

void UOrbitWeaponSystem::LogTelemetry(const FString& Message) const
{
    // In a real project this might send data to a remote server.
    UE_LOG(LogTemp, Log, TEXT("[OrbitWeapon Telemetry] %s"), *Message);
}

#undef LOCTEXT_NAMESPACE
