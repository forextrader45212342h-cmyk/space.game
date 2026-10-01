#include "OrbitWeaponComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"

UOrbitWeaponComponent::UOrbitWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UOrbitWeaponComponent::TryFire(
    const FVector& Start,
    const FVector& Direction)
{
    UWorld* World = GetWorld();

    if (!World ||
        AmmoInMagazine <= 0)
    {
        return false;
    }

    const float Now =
        World->GetTimeSeconds();

    if (Now - LastFireTime <
        FireCooldown)
    {
        return false;
    }

    LastFireTime = Now;

    --AmmoInMagazine;

    FHitResult Hit;

    FCollisionQueryParams Params(
        SCENE_QUERY_STAT(
            OrbitPistolTrace),
        true);

    Params.AddIgnoredActor(
        GetOwner());

    const FVector End =
        Start
        +
        Direction.GetSafeNormal()
        *
        100000.0f;

    if (World->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params))
    {
        if (AActor* HitActor =
            Hit.GetActor())
        {
            FPointDamageEvent
                DamageEvent;

            HitActor->TakeDamage(
                Damage,
                DamageEvent,
                nullptr,
                GetOwner());
        }
    }

    return true;
}

bool UOrbitWeaponComponent::Reload()
{
    if (AmmoInMagazine >=
        MagazineSize ||
        ReserveAmmo <= 0)
    {
        return false;
    }

    const int32 Needed =
        MagazineSize -
        AmmoInMagazine;

    const int32 Loaded =
        FMath::Min(
            Needed,
            ReserveAmmo);

    AmmoInMagazine += Loaded;

    ReserveAmmo -= Loaded;

    return Loaded > 0;
}
