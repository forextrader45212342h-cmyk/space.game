// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  UOrbitSaveGame
 *
 *  A simple SaveGame subclass that stores the player's
 *  fuel level, score, current coordinates and a list of
 *  unlocked ships.  All properties are exposed to Blueprints
 *  so they can be read or modified from the editor or
 *  gameplay scripts.
 */
UCLASS(BlueprintType, Category = "Orbit")
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor – initialise members to safe defaults. */
	UOrbitSaveGame()
		: Fuel(0.f)
		, Score(0)
		, Coordinates(FVector::ZeroVector)
	{
	}

	/** Current fuel level of the player. */
	UPROPERTY(BlueprintReadWrite, Category = "Orbit|Player")
	float Fuel;

	/** Current score of the player. */
	UPROPERTY(BlueprintReadWrite, Category = "Orbit|Player")
	int32 Score;

	/** Current world coordinates of the player. */
	UPROPERTY(BlueprintReadWrite, Category = "Orbit|Player")
	FVector Coordinates;

	/** List of ship identifiers that the player has unlocked. */
	UPROPERTY(BlueprintReadWrite, Category = "Orbit|Player")
	TArray<FString> UnlockedShips;
};
