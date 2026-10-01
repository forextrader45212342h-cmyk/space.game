#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbitGameState.generated.h"

UENUM(BlueprintType)
enum class EOrbitGameMode : uint8
{
    Planet UMETA(DisplayName="Planet"),
    Atmosphere UMETA(DisplayName="Atmosphere"),
    Orbit UMETA(DisplayName="Orbit"),
    DeepSpace UMETA(DisplayName="Deep Space"),
    Moon UMETA(DisplayName="Moon")
};

UCLASS()
class PROJECTORBIT_API AOrbitGameState : public AGameStateBase
{
    GENERATED_BODY()

public:

    AOrbitGameState();

    UPROPERTY(BlueprintReadOnly, Category="Orbit")
    EOrbitGameMode CurrentMode;

    UPROPERTY(BlueprintReadOnly, Category="Orbit")
    FString CurrentLocation;

    UPROPERTY(BlueprintReadOnly, Category="Orbit")
    float DistanceFromPlanet;

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void SetGameMode(EOrbitGameMode NewMode);

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void SetLocation(const FString& NewLocation);

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void SetDistanceFromPlanet(float NewDistance);
};
