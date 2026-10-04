// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  A simple save game class that stores the player's fuel, score,
 *  current world coordinates and a list of unlocked ships.
 */
UCLASS(BlueprintType, Category = "Orbit")
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame();

	/** Current fuel level of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Player")
	float Fuel;

	/** Current score of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Player")
	int32 Score;

	/** Current world coordinates of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Player")
	FVector Coordinates;

	/** List of ship identifiers that have been unlocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Player")
	TArray<FString> UnlockedShips;
};
