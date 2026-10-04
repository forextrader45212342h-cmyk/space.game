// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  Stores persistent game data such as fuel, score, ship coordinates and unlocked ships.
 *  This class is intended to be used with the UE5 SaveGame system.
 */
UCLASS(BlueprintType, Category = "Orbit|Save")
class ORBIT_API UOrbitSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    /** Default constructor */
    UOrbitSaveGame()
        : Fuel(0)
        , Score(0)
    {}

    /** Current fuel level */
    UPROPERTY(BlueprintReadWrite, Category = "Orbit|Save")
    int32 Fuel;

    /** Current score */
    UPROPERTY(BlueprintReadWrite, Category = "Orbit|Save")
    int32 Score;

    /** Positions of all ships that have been spawned or visited */
    UPROPERTY(BlueprintReadWrite, Category = "Orbit|Save")
    TArray<FVector> ShipCoordinates;

    /** Names of ships that have been unlocked by the player */
    UPROPERTY(BlueprintReadWrite, Category = "Orbit|Save")
    TArray<FName> UnlockedShips;

    /** Helper to add a new ship coordinate */
    UFUNCTION(BlueprintCallable, Category = "Orbit|Save")
    void AddShipCoordinate(const FVector& NewCoord)
    {
        ShipCoordinates.Add(NewCoord);
    }

    /** Helper to unlock a new ship */
    UFUNCTION(BlueprintCallable, Category = "Orbit|Save")
    void UnlockShip(const FName& ShipName)
    {
        if (!UnlockedShips.Contains(ShipName))
        {
            UnlockedShips.Add(ShipName);
        }
    }
};
