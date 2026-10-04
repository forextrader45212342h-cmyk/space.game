// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  Simple save game class that stores the player's fuel, score, current
 *  world coordinates and the list of ships that have been unlocked.
 *
 *  All properties are marked with UPROPERTY so that the engine can
 *  automatically serialize them when the game is saved or loaded.
 */
UCLASS(BlueprintType, MinimalAPI)
class UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor – initialise with sensible defaults. */
	UOrbitSaveGame()
		: Fuel(0.f)
		, Score(0)
		, Coordinates(FVector::ZeroVector)
	{
	}

	/** Current amount of fuel the player has. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float Fuel;

	/** Total score accumulated by the player. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 Score;

	/** Player's current world position. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector Coordinates;

	/** List of ship identifiers that the player has unlocked. */
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FString> UnlockedShips;
};
