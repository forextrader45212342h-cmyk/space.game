#include "OrbitPedestrianImpactComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UOrbitPedestrianImpactComponent::UOrbitPedestrianImpactComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UOrbitPedestrianImpactComponent::HandleVehicleImpact(
    AActor* VehicleActor,
    FVector ImpactPoint,
    FVector VehicleVelocity)
{
    if (bUnconscious)
        return false;

    if (!VehicleActor || !GetOwner())
        return false;

    const float Speed =
        VehicleVelocity.Size();

    if (Speed < KnockoutSpeedCmPerSec)
        return false;

    ACharacter* Character =
        Cast<ACharacter>(GetOwner());

    if (!Character)
        return false;

    USkeletalMeshComponent* Mesh =
        Character->GetMesh();

    if (!Mesh)
        return false;

    const FVector Direction =
        VehicleVelocity.GetSafeNormal();

    const float Impulse =
        FMath::Max(
            MinimumImpactImpulse,
            Speed * 180.0f
        );

    bUnconscious = true;

    if (UCharacterMovementComponent* Movement =
        Character->GetCharacterMovement())
    {
        Movement->StopMovementImmediately();
        Movement->DisableMovement();
    }

    Mesh->SetCollisionProfileName(
        TEXT("Ragdoll")
    );

    Mesh->SetSimulatePhysics(true);

    Mesh->WakeAllRigidBodies();

    Mesh->AddImpulseAtLocation(
        Direction * Impulse,
        ImpactPoint
    );

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            RecoveryTimer,
            this,
            &UOrbitPedestrianImpactComponent::RecoverFromKnockdown,
            RecoveryTimeSeconds,
            false
        );
    }

    return true;
}

void UOrbitPedestrianImpactComponent::RecoverFromKnockdown()
{
    ACharacter* Character =
        Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    if (USkeletalMeshComponent* Mesh =
        Character->GetMesh())
    {
        Mesh->SetSimulatePhysics(false);

        Mesh->SetCollisionProfileName(
            TEXT("CharacterMesh")
        );
    }

    if (UCharacterMovementComponent* Movement =
        Character->GetCharacterMovement())
    {
        Movement->SetMovementMode(
            MOVE_Walking
        );
    }

    bUnconscious = false;
}
