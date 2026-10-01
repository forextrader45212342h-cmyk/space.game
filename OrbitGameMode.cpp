#include "OrbitGameMode.h"
#include "Engine/World.h"

AOrbitGameMode::AOrbitGameMode()
{
    bGameStarted = false;
}

void AOrbitGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AOrbitGameMode::StartGame()
{
    bGameStarted = true;
}

void AOrbitGameMode::StartSpaceExploration()
{
    if (!bGameStarted)
    {
        StartGame();
    }
}

void AOrbitGameMode::StartPlanetExploration()
{
    if (!bGameStarted)
    {
        StartGame();
    }
}

bool AOrbitGameMode::IsGameStarted() const
{
    return bGameStarted;
}
