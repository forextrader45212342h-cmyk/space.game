// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  A simple save game class that stores the player's fuel, score,
 *  current coordinates, and a list of unlocked ships.
 */
UCLASS()
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame()
		: Fuel(0.f)
		, Score(0)
		, CurrentCoordinates(FVector::ZeroVector)
	{
	}

	/** Current fuel level of the player */
	UPROPERTY(VisibleAnywhere, Category = "Player")
	float Fuel;

	/** Current score of the player */
	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 Score;

	/** Current world coordinates of the player */
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FVector CurrentCoordinates;

	/** List of ship identifiers that the player has unlocked */
	UPROPERTY(VisibleAnywhere, Category = "Player")
	TArray<FString> UnlockedShips;
};
