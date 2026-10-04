// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GameplaySaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  UOrbitSaveGame
 *
 *  A simple save‑game class that stores the player's fuel, score,
 *  current world coordinates and the list of ships that have been
 *  unlocked.  All properties are marked with `SaveGame` so that
 *  the engine will automatically serialize them when the game is
 *  saved or loaded.
 */
UCLASS(BlueprintType, Blueprintable, Category = "Orbit")
class ORBIT_API UOrbitSaveGame : public UGameplaySaveGame
{
    GENERATED_BODY()

public:
    /** Default constructor – initialise with sane defaults. */
    UOrbitSaveGame()
        : Fuel(0.f)
        , Score(0)
        , Coordinates(FVector::ZeroVector)
    {}

    /** Current amount of fuel the player has. */
    UPROPERTY(VisibleAnywhere, Category = "Orbit|State", SaveGame)
    float Fuel;

    /** Player's accumulated score. */
    UPROPERTY(VisibleAnywhere, Category = "Orbit|State", SaveGame)
    int32 Score;

    /** Current world coordinates of the player. */
    UPROPERTY(VisibleAnywhere, Category = "Orbit|State", SaveGame)
    FVector Coordinates;

    /** List of ship identifiers that the player has unlocked. */
    UPROPERTY(VisibleAnywhere, Category = "Orbit|State", SaveGame)
    TArray<FName> UnlockedShips;

    /** Unlock a new ship.  If the ship is already unlocked it is ignored. */
    UFUNCTION(BlueprintCallable, Category = "Orbit|Gameplay")
    void UnlockShip(const FName& ShipName)
    {
        if (!UnlockedShips.Contains(ShipName))
        {
            UnlockedShips.Add(ShipName);
        }
    }

    /** Check whether a ship has already been unlocked. */
    UFUNCTION(BlueprintCallable, Category = "Orbit|Gameplay")
    bool IsShipUnlocked(const FName& ShipName) const
    {
        return UnlockedShips.Contains(ShipName);
    }

    /** Reset the save data to its default state. */
    UFUNCTION(BlueprintCallable, Category = "Orbit|Gameplay")
    void Reset()
    {
        Fuel = 0.f;
        Score = 0;
        Coordinates = FVector::ZeroVector;
        UnlockedShips.Empty();
    }
};
