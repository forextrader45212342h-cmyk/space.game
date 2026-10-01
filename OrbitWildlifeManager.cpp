#include "OrbitWildlifeManager.h"
#include "Kismet/KismetMathLibrary.h"

UOrbitWildlifeManager::UOrbitWildlifeManager()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitWildlifeManager::BeginPlay()
{
    Super::BeginPlay();
}

void UOrbitWildlifeManager::RegisterWildlife(
    AActor* Actor,
    float Speed)
{
    if (!IsValid(Actor))
        return;

    FOrbitWildlifeAgent Agent;

    Agent.Actor = Actor;

    Agent.Speed =
        FMath::Max(
            1.0f,
            Speed);

    Agent.Target =
        Actor->GetActorLocation()
        +
        UKismetMathLibrary::
        RandomUnitVector()
        *
        WanderRadius;

    Agents.Add(Agent);
}

void UOrbitWildlifeManager::UnregisterWildlife(
    AActor* Actor)
{
    Agents.RemoveAll(
        [Actor](
            const FOrbitWildlifeAgent& Agent)
        {
            return Agent.Actor == Actor;
        });
}

void UOrbitWildlifeManager::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction*
    ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction);

    Accumulator += DeltaTime;

    if (Accumulator >= UpdateInterval)
    {
        UpdateAgents(Accumulator);
        Accumulator = 0.0f;
    }
}

void UOrbitWildlifeManager::UpdateAgents(
    float DeltaSeconds)
{
    for (FOrbitWildlifeAgent& Agent : Agents)
    {
        if (!IsValid(Agent.Actor))
            continue;

        const FVector Location =
            Agent.Actor->GetActorLocation();

        FVector ToTarget =
            Agent.Target -
            Location;

        if (ToTarget.SizeSquared() < 50000.0f)
        {
            Agent.Target =
                Location
                +
                UKismetMathLibrary::
                RandomUnitVector()
                *
                WanderRadius;

            ToTarget =
                Agent.Target -
                Location;
        }

        const FVector Direction =
            ToTarget.GetSafeNormal();

        const FVector NewLocation =
            Location
            +
            Direction *
            Agent.Speed *
            DeltaSeconds;

        Agent.Actor->SetActorLocation(
            NewLocation,
            true);
    }
}
