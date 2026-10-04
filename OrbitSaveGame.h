// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  USaveGame subclass that stores the player's persistent data.
 *
 *  - Fuel: Current amount of fuel the player has.
 *  - Score: Total score accumulated.
 *  - Coordinates: Current world position of the player.
 *  - UnlockedShips: List of ship identifiers that the player has unlocked.
 *
 *  All properties are marked with UPROPERTY so that UE5's serialization
 *  system will automatically persist them when the game is saved.
 */
UCLASS(BlueprintType, Category = "Orbit")
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame();

	/** Reset all values to their defaults */
	UFUNCTION(BlueprintCallable, Category = "Orbit|SaveGame")
	void Reset();

	/** Set the player's current fuel amount */
	UFUNCTION(BlueprintCallable, Category = "Orbit|SaveGame")
	void SetFuel(int32 NewFuel);

	/** Get the player's current fuel amount */
	UFUNCTION(BlueprintPure, Category = "Orbit|SaveGame")
	int32 GetFuel() const;

	/** Set the player's current score */
	UFUNCTION(BlueprintCallable, Category = "Orbit|SaveGame")
	void SetScore(int32 NewScore);

	/** Get the player's current score */
	UFUNCTION(BlueprintPure, Category = "Orbit|SaveGame")
	int32 GetScore() const;

	/** Set the player's current world coordinates */
	UFUNCTION(BlueprintCallable, Category = "Orbit|SaveGame")
	void SetCoordinates(const FVector& NewCoords);

	/** Get the player's current world coordinates */
	UFUNCTION(BlueprintPure, Category = "Orbit|SaveGame")
	FVector GetCoordinates() const;

	/** Add a ship to the unlocked list (if not already present) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|SaveGame")
	void UnlockShip(const FName& ShipName);

	/** Check if a ship is unlocked */
	UFUNCTION(BlueprintPure, Category = "Orbit|SaveGame")
	bool IsShipUnlocked(const FName& ShipName) const;

	/** Get the full list of unlocked ships */
	UFUNCTION(BlueprintPure, Category = "Orbit|SaveGame")
	const TArray<FName>& GetUnlockedShips() const;

private:
	/** Current fuel amount */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|SaveGame")
	int32 Fuel;

	/** Current score */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|SaveGame")
	int32 Score;

	/** Current world coordinates */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|SaveGame")
	FVector Coordinates;

	/** List of ship identifiers that have been unlocked */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|SaveGame")
	TArray<FName> UnlockedShips;
};
