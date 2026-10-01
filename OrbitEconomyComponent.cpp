#include "OrbitEconomyComponent.h"

UOrbitEconomyComponent::UOrbitEconomyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitEconomyComponent::AddReward(
    int64 Amount)
{
    if (Amount <= 0)
        return;

    Cash += Amount;

    LifetimeEarned += Amount;
}

bool UOrbitEconomyComponent::TrySpend(
    int64 Amount)
{
    if (!CanAfford(Amount))
        return false;

    Cash -= Amount;

    return true;
}

bool UOrbitEconomyComponent::CanAfford(
    int64 Amount) const
{
    return
        Amount >= 0 &&
        Cash >= Amount;
}
