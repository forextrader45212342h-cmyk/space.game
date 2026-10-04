// OrbitWeaponSystem.cpp
// Implements laser firing, line‑trace damage, and telemetry for the Orbit weapon system.

#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // UOrbitWeaponSystem
    // ------------------------------------------------------------------

    UOrbitWeaponSystem::UOrbitWeaponSystem()
    {
        // Default values
        BaseDamage   = 25.f;
        DamageType   = UDamageType::StaticClass();
        ImpactEffect = nullptr;
    }

    // Fire a laser from Start to End.  Performs a line‑trace, applies damage,
    // spawns an impact effect, and logs telemetry.
    void UOrbitWeaponSystem::FireLaser(const FVector& Start, const FVector& End)
    {
        if (!GetWorld())
        {
            UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem::FireLaser - No world context"));
            return;
        }

        // Prepare trace parameters
        FHitResult HitResult;
        FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(OrbitLaserTrace), true);
        TraceParams.AddIgnoredActor(GetOwner());
        TraceParams.bTraceComplex = true;

        // Perform the line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,
            TraceParams
        );

        // Debug line for visual feedback
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            bHit ? FColor::Red : FColor::Green,
            false,
            1.0f,
            0,
            2.0f
        );

        if (bHit)
        {
            // ------------------------------------------------------------------
            // Telemetry
            // ------------------------------------------------------------------
            const AActor* HitActor = HitResult.GetActor();
            const FVector HitLocation = HitResult.Location;
            const FVector HitNormal   = HitResult.Normal;

            UE_LOG(
                LogTemp,
                Log,
                TEXT("[Orbit] Laser hit '%s' at %s (Normal: %s)"),
                HitActor ? *HitActor->GetName() : TEXT("None"),
                *HitLocation.ToString(),
                *HitNormal.ToString()
            );

            // ------------------------------------------------------------------
            // Damage
            // ------------------------------------------------------------------
            const FVector ShotDirection = (End - Start).GetSafeNormal();

            UGameplayStatics::ApplyPointDamage(
                HitActor,
                BaseDamage,
                ShotDirection,
                HitResult,
                GetOwner()->GetInstigatorController(),
                this,
                DamageType
            );

            // ------------------------------------------------------------------
            // Impact effect
            // ------------------------------------------------------------------
            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(),
                    ImpactEffect,
                    HitLocation,
                    HitNormal.Rotation(),
                    true
                );
            }
        }
    }

    // ------------------------------------------------------------------
    // Configuration helpers
    // ------------------------------------------------------------------

    void UOrbitWeaponSystem::SetBaseDamage(float Damage)
    {
        BaseDamage = Damage;
    }

    void UOrbitWeaponSystem::SetDamageType(TSubclassOf<UDamageType> InDamageType)
    {
        DamageType = InDamageType;
    }

    void UOrbitWeaponSystem::SetImpactEffect(UParticleSystem* Effect)
    {
        ImpactEffect = Effect;
    }
}
