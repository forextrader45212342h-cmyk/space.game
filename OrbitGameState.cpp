#include "OrbitGameState.h"

AOrbitGameState::AOrbitGameState()
{
    CurrentMode = EOrbitGameMode::Planet;
    CurrentLocation = TEXT("Unknown");
    DistanceFromPlanet = 0.0f;
}

void AOrbitGameState::SetGameMode(EOrbitGameMode NewMode)
{
    CurrentMode = NewMode;
}

void AOrbitGameState::SetLocation(const FString& NewLocation)
{
    CurrentLocation = NewLocation;
}

void AOrbitGameState::SetDistanceFromPlanet(float NewDistance)
{
    DistanceFromPlanet = FMath::Max(0.0f, NewDistance);
}
