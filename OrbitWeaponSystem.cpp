// OrbitWeaponSystem.cpp
#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitTelemetry.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // Fire a laser from the weapon's muzzle.  The laser is a simple
    // line‑trace that applies damage to the first hit actor and logs
    // telemetry data about the hit.
    // ------------------------------------------------------------------
    void OrbitWeaponSystem::FireLaser()
    {
        if (!WeaponOwner || !WeaponOwner->GetWorld())
        {
            UE_LOG(LogOrbit, Warning, TEXT("OrbitWeaponSystem::FireLaser - Invalid owner or world"));
            return;
        }

        // 1. Determine start and end points
        const FVector MuzzleLocation = GetMuzzleWorldLocation();
        const FVector ForwardVector = GetMuzzleForwardVector();
        const float MaxRange = WeaponConfig.LaserRange;

        const FVector EndLocation = MuzzleLocation + ForwardVector * MaxRange;

        // 2. Setup trace parameters
        FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(LaserTrace), true, WeaponOwner);
        TraceParams.bReturnPhysicalMaterial = false;
        TraceParams.bTraceComplex = true;

        // 3. Perform the line trace
        FHitResult HitResult;
        const bool bHit = WeaponOwner->GetWorld()->LineTraceSingleByChannel(
            HitResult,
            MuzzleLocation,
            EndLocation,
            ECC_Visibility,
            TraceParams
        );

        // 4. Draw debug line (optional)
        #if WITH_EDITOR
        const FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            WeaponOwner->GetWorld(),
            MuzzleLocation,
            EndLocation,
            LineColor,
            false,
            1.0f,
            0,
            1.0f
        );
        #endif

        // 5. If we hit something, apply damage and log telemetry
        if (bHit && HitResult.GetActor())
        {
            // Apply damage
            const float DamageAmount = WeaponConfig.LaserDamage;
            UGameplayStatics::ApplyPointDamage(
                HitResult.GetActor(),
                DamageAmount,
                ForwardVector,
                HitResult,
                WeaponOwner->GetInstigatorController(),
                this,
                DamageTypeClass
            );

            // Telemetry: record hit data
            Telemetry::RecordLaserHit(
                WeaponOwner,
                HitResult.GetActor(),
                HitResult.ImpactPoint,
                DamageAmount
            );

            // Optional: spawn impact effect
            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(
                    WeaponOwner->GetWorld(),
                    ImpactEffect,
                    HitResult.ImpactPoint,
                    HitResult.ImpactNormal.Rotation()
                );
            }
        }
        else
        {
            // Telemetry: record miss
            Telemetry::RecordLaserMiss(
                WeaponOwner,
                EndLocation
            );
        }
    }

    // ------------------------------------------------------------------
    // Helper to get the world-space location of the weapon's muzzle.
    // ------------------------------------------------------------------
    FVector OrbitWeaponSystem::GetMuzzleWorldLocation() const
    {
        if (MuzzleSocketName.IsNone())
        {
            return WeaponOwner->GetActorLocation();
        }

        return WeaponOwner->GetActorTransform().TransformPosition(
            WeaponOwner->GetRootComponent()->GetSocketLocation(MuzzleSocketName)
        );
    }

    // ------------------------------------------------------------------
    // Helper to get the forward vector of the muzzle.
    // ------------------------------------------------------------------
    FVector OrbitWeaponSystem::GetMuzzleForwardVector() const
    {
        if (MuzzleSocketName.IsNone())
        {
            return WeaponOwner->GetActorForwardVector();
        }

        return WeaponOwner->GetActorTransform().TransformVector(
            WeaponOwner->GetRootComponent()->GetSocketRotation(MuzzleSocketName).Vector()
        );
    }
}
