// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  A simple save game class that stores the player's fuel, score,
 *  current coordinates, and a list of unlocked ships.
 *
 *  All properties are exposed to the editor and Blueprints so they
 *  can be inspected or modified during gameplay or from the editor.
 */
UCLASS()
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame();

	/** Current amount of fuel the player has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	float Fuel;

	/** Current score of the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	int32 Score;

	/** Current world location of the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	FVector PlayerLocation;

	/** List of ship identifiers that the player has unlocked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	TArray<FName> UnlockedShips;
};
