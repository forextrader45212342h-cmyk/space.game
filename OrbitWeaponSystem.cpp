// OrbitWeaponSystem.cpp
// ---------------
// UE5 C++ implementation of a laser weapon system that performs a line‑trace,
// applies damage to hit actors, and records telemetry data about the impact.

#include "OrbitWeaponSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "OrbitTelemetry.h"

//////////////////////////////////////////////////////////////////////////
// UOrbitWeaponSystem

UOrbitWeaponSystem::UOrbitWeaponSystem()
{
    // Default values
    DamageAmount = 25.0f;
    TraceDistance = 10000.0f;
    bDebugDraw = true;
}

void UOrbitWeaponSystem::FireLaser()
{
    if (!MuzzleComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbitWeaponSystem: MuzzleComponent is null."));
        return;
    }

    const FVector Start = MuzzleComponent->GetComponentLocation();
    const FVector Forward = MuzzleComponent->GetForwardVector();
    const FVector End   = Start + Forward * TraceDistance;

    FHitResult HitResult;
    const FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner()); // Don't hit ourselves

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );

    // Debug visualisation
    if (bDebugDraw)
    {
        const FColor HitColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(GetWorld(), Start, End, HitColor, false, 1.0f, 0, 1.0f);
        if (bHit)
        {
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 8.0f, 12, FColor::Yellow, false, 1.0f);
        }
    }

    if (bHit)
    {
        ApplyDamage(HitResult);
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

    UDamageType const* DamageType = UDamageType::StaticClass()->GetDefaultObject<UDamageType>();
    UGameplayStatics::ApplyPointDamage(
        HitActor,
        DamageAmount,
        Hit.TraceStart - Hit.TraceEnd, // Direction
        Hit,
        GetOwner()->GetInstigatorController(),
        GetOwner(),
        DamageType
    );
}

void UOrbitWeaponSystem::RecordTelemetry(const FHitResult& Hit)
{
    if (!UTelemetrySubsystem::IsAvailable())
    {
        return;
    }

    UTelemetrySubsystem* Telemetry = UTelemetrySubsystem::Get();
    if (!Telemetry)
    {
        return;
    }

    FTelemetryEvent Event;
    Event.EventName = TEXT("LaserHit");
    Event.Timestamp = FDateTime::UtcNow();

    // Basic hit data
    Event.Data.Add(TEXT("HitActor"), Hit.GetActor()->GetName());
    Event.Data.Add(TEXT("HitLocation"), Hit.ImpactPoint.ToString());
    Event.Data.Add(TEXT("DamageDealt"), FString::SanitizeFloat(DamageAmount));

    // If the hit actor implements a health interface, query remaining health
    if (Hit.GetActor()->Implements<UHealthInterface>())
    {
        float RemainingHealth = IHealthInterface::Execute_GetHealth(Hit.GetActor());
        Event.Data.Add(TEXT("RemainingHealth"), FString::SanitizeFloat(RemainingHealth));
    }

    Telemetry->RecordEvent(Event);
}
