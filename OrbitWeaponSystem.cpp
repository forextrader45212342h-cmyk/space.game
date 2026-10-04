// OrbitWeaponSystem.cpp
// Implements laser firing, line‑trace damage, and telemetry

#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

namespace Orbit
{
    // --------------------------------------------------------------------
    // FireLaser
    // --------------------------------------------------------------------
    void OrbitWeaponSystem::FireLaser()
    {
        // Validate world and owner
        if (!GetWorld() || !Owner)
        {
            UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem::FireLaser - Invalid world or owner"));
            return;
        }

        // Get muzzle location and forward vector
        const FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();
        const FVector MuzzleForward = MuzzleComponent->GetForwardVector();

        // Compute end point
        const FVector EndLocation = MuzzleLocation + (MuzzleForward * MaxRange);

        // Setup query params
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Owner);
        QueryParams.bTraceComplex = true;
        QueryParams.bReturnPhysicalMaterial = false;

        // Perform line trace
        FHitResult HitResult;
        const bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            MuzzleLocation,
            EndLocation,
            ECC_Visibility,
            QueryParams
        );

        // Debug line
        const FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(
            GetWorld(),
            MuzzleLocation,
            bHit ? HitResult.ImpactPoint : EndLocation,
            LineColor,
            false,
            2.0f,
            0,
            1.0f
        );

        // Telemetry: log basic info
        UE_LOG(LogTemp, Log, TEXT("OrbitWeaponSystem::FireLaser - Fired from %s"),
            *Owner->GetName());

        if (bHit)
        {
            // Telemetry: hit details
            const float Distance = (HitResult.ImpactPoint - MuzzleLocation).Size();
            UE_LOG(LogTemp, Log, TEXT("  Hit %s at %.2f units"),
                *HitResult.GetActor()->GetName(), Distance);

            // Damage application
            float DamageApplied = DamageAmount;

            // If the hit actor implements a custom damage interface, let it handle damage
            if (HitResult.GetActor()->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
            {
                IDamageable::Execute_ApplyDamage(HitResult.GetActor(), DamageApplied);
            }
            else
            {
                // Fallback to generic damage system
                UGameplayStatics::ApplyDamage(
                    HitResult.GetActor(),
                    DamageApplied,
                    Owner->GetInstigatorController(),
                    Owner,
                    UDamageType::StaticClass()
                );
            }

            // Telemetry: damage applied
            UE_LOG(LogTemp, Log, TEXT("  Applied %.2f damage to %s"),
                DamageApplied, *HitResult.GetActor()->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("  No hit detected"));
        }
    }

    // --------------------------------------------------------------------
    // TickComponent
    // --------------------------------------------------------------------
    void OrbitWeaponSystem::TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction)
    {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

        // Example: auto fire every 0.5 seconds
        FireTimer += DeltaTime;
        if (FireTimer >= FireInterval)
        {
            FireTimer = 0.0f;
            FireLaser();
        }
    }

    // --------------------------------------------------------------------
    // Setup
    // --------------------------------------------------------------------
    void OrbitWeaponSystem::BeginPlay()
    {
        Super::BeginPlay();

        Owner = GetOwner();
        if (!MuzzleComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem::BeginPlay - MuzzleComponent not set"));
        }
    }
}
