// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  UOrbitSaveGame
 *  ----------------
 *  A simple save game class that stores the player's fuel, score, current location
 *  and the list of ships that have been unlocked.  All properties are exposed to
 *  Blueprints and can be edited in the editor or via C++.
 */
UCLASS(BlueprintType, MinimalAPI)
class UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame()
		: Fuel(0.f)
		, Score(0)
		, CurrentLocation(FVector::ZeroVector)
	{
	}

	/** Current fuel level of the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	float Fuel;

	/** Current score of the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	int32 Score;

	/** Current world location of the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	FVector CurrentLocation;

	/** List of ship identifiers that the player has unlocked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	TArray<FName> UnlockedShips;

	/** Adds a ship to the unlocked list if it isn't already present. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Save")
	void UnlockShip(const FName& ShipName)
	{
		if (!UnlockedShips.Contains(ShipName))
		{
			UnlockedShips.Add(ShipName);
		}
	}

	/** Clears all saved data (useful for debugging or a "New Game" option). */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Save")
	void Reset()
	{
		Fuel = 0.f;
		Score = 0;
		CurrentLocation = FVector::ZeroVector;
		UnlockedShips.Empty();
	}
};
