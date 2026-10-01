#include "OrbitVehicleDynamicsComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

UOrbitVehicleDynamicsComponent::UOrbitVehicleDynamicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UOrbitVehicleDynamicsComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        if (UPrimitiveComponent* Body =
            Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
        {
            SmoothedVelocity = Body->GetPhysicsLinearVelocity();
        }
    }
}

void UOrbitVehicleDynamicsComponent::SetThrottle(float NewThrottle)
{
    ThrottleInput = FMath::Clamp(NewThrottle, -1.0f, 1.0f);
}

void UOrbitVehicleDynamicsComponent::SetBraking(float NewBrake)
{
    BrakeInput = FMath::Clamp(NewBrake, 0.0f, 1.0f);
}

void UOrbitVehicleDynamicsComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (DeltaTime <= KINDA_SMALL_NUMBER || !GetOwner())
        return;

    UPrimitiveComponent* Body =
        Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

    if (!Body || !Body->IsSimulatingPhysics())
        return;

    const FVector CurrentVelocity =
        Body->GetPhysicsLinearVelocity();

    const FVector Forward =
        GetOwner()->GetActorForwardVector().GetSafeNormal();

    const float SignedSpeed =
        FVector::DotProduct(CurrentVelocity, Forward);

    const float SpeedLimit =
        ThrottleInput >= 0.0f
        ? MaxForwardSpeedCmPerSec
        : MaxForwardSpeedCmPerSec * ReverseSpeedFraction;

    const float TargetSpeed =
        ThrottleInput * SpeedLimit;

    FVector TargetVelocity =
        Forward * TargetSpeed;

    if (bAffectOnlyForwardAxis)
    {
        const FVector LateralVelocity =
            CurrentVelocity - Forward * SignedSpeed;

        TargetVelocity += LateralVelocity;
    }

    if (BrakeInput > 0.0f)
    {
        TargetVelocity =
            FMath::VInterpTo(
                CurrentVelocity,
                FVector::ZeroVector,
                DeltaTime,
                DecelerationResponse *
                FMath::Max(BrakeInput, 0.01f)
            );
    }
    else
    {
        const bool bDecelerating =
            FMath::Abs(TargetSpeed) <
            FMath::Abs(SignedSpeed);

        const float Response =
            bDecelerating
            ? DecelerationResponse
            : AccelerationResponse;

        TargetVelocity =
            FMath::VInterpTo(
                CurrentVelocity,
                TargetVelocity,
                DeltaTime,
                Response
            );
    }

    SmoothedVelocity = TargetVelocity;

    const FVector RequiredAcceleration =
        (TargetVelocity - CurrentVelocity) / DeltaTime;

    Body->AddForce(
        RequiredAcceleration * Body->GetMass(),
        NAME_None,
        true
    );
}
