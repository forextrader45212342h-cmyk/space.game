#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitGameMode.generated.h"

UCLASS()
class PROJECTORBIT_API AOrbitGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    AOrbitGameMode();

protected:

    virtual void BeginPlay() override;

public:

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void StartSpaceExploration();

    UFUNCTION(BlueprintCallable, Category="Orbit")
    void StartPlanetExploration();

    UFUNCTION(BlueprintPure, Category="Orbit")
    bool IsGameStarted() const;

private:

    bool bGameStarted;
};
