#include "OrbitPoliceAIComponent.h"

UOrbitPoliceAIComponent::
UOrbitPoliceAIComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitPoliceAIComponent::Dispatch(
    AActor* TargetActor)
{
    Target = TargetActor;

    bDispatched =
        IsValid(TargetActor);
}

FVector
UOrbitPoliceAIComponent::
GetTargetLocation() const
{
    return Target.IsValid()
        ?
        Target->GetActorLocation()
        :
        FVector::ZeroVector;
}

bool
UOrbitPoliceAIComponent::
IsTargetCaptured() const
{
    if (!Target.IsValid() ||
        !GetOwner())
    {
        return false;
    }

    return
        FVector::Dist(
            GetOwner()->GetActorLocation(),
            Target->GetActorLocation())
        <= CaptureDistance;
}

void
UOrbitPoliceAIComponent::
TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction*
    ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction);

    if (!bDispatched ||
        !Target.IsValid())
    {
        return;
    }
}
