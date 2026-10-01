#include "OrbitAmbientMoverComponent.h"

#include "Components/SplineComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

UOrbitAmbientMoverComponent::UOrbitAmbientMoverComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UOrbitAmbientMoverComponent::BeginPlay()
{
    Super::BeginPlay();

    ResetToSplineStart();

    if (AActor* Owner = GetOwner())
    {
        if (UPrimitiveComponent* Primitive =
            Cast<UPrimitiveComponent>(
                Owner->GetRootComponent()))
        {
            SavedCollision =
                Primitive->GetCollisionEnabled();

            SavedResponses =
                Primitive->GetCollisionResponseToChannels();
        }
    }
}

void UOrbitAmbientMoverComponent::ResetToSplineStart()
{
    DistanceAlongSpline = 0.0f;
}

void UOrbitAmbientMoverComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction
    );

    if (DeltaTime <= KINDA_SMALL_NUMBER)
        return;

    if (!FollowSpline || !GetOwner())
        return;

    const float SplineLength =
        FollowSpline->GetSplineLength();

    if (SplineLength <= KINDA_SMALL_NUMBER)
        return;

    bool bFar = false;

    if (IsValid(FocusActor))
    {
        const float DistanceSquared =
            FVector::DistSquared(
                GetOwner()->GetActorLocation(),
                FocusActor->GetActorLocation()
            );

        bFar =
            DistanceSquared >
            FMath::Square(ActiveDistanceCm);
    }

    if (bFar)
    {
        if (!bWasFar && bDisableCollisionWhenFar)
        {
            if (UPrimitiveComponent* Primitive =
                Cast<UPrimitiveComponent>(
                    GetOwner()->GetRootComponent()))
            {
                Primitive->SetCollisionEnabled(
                    ECollisionEnabled::NoCollision
                );
            }
        }

        bWasFar = true;
        return;
    }

    if (bWasFar && bDisableCollisionWhenFar)
    {
        if (UPrimitiveComponent* Primitive =
            Cast<UPrimitiveComponent>(
                GetOwner()->GetRootComponent()))
        {
            Primitive->SetCollisionEnabled(
                SavedCollision
            );

            Primitive->SetCollisionResponseToChannels(
                SavedResponses
            );
        }
    }

    bWasFar = false;

    DistanceAlongSpline =
        FMath::Fmod(
            DistanceAlongSpline +
            SpeedCmPerSec * DeltaTime,
            SplineLength
        );

    const FVector NewLocation =
        FollowSpline->GetLocationAtDistanceAlongSpline(
            DistanceAlongSpline,
            ESplineCoordinateSpace::World
        );

    const FVector LookPoint =
        FollowSpline->GetLocationAtDistanceAlongSpline(
            FMath::Fmod(
                DistanceAlongSpline +
                LookAheadDistanceCm,
                SplineLength
            ),
            ESplineCoordinateSpace::World
        );

    const FRotator NewRotation =
        (LookPoint - NewLocation).Rotation();

    GetOwner()->SetActorLocationAndRotation(
        NewLocation,
        NewRotation,
        false,
        nullptr,
        ETeleportType::TeleportPhysics
    );
}
