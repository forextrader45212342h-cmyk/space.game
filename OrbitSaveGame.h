// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  USaveGame subclass that stores the player's fuel, score, current coordinates,
 *  and the list of ships that have been unlocked.
 */
UCLASS(BlueprintType)
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame()
	{
		Fuel = 0.0f;
		Score = 0;
		Coordinates = FVector::ZeroVector;
		UnlockedShips.Empty();
	}

	/** Current fuel level of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float Fuel;

	/** Current score of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 Score;

	/** Current world coordinates of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	FVector Coordinates;

	/** List of ship identifiers that have been unlocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FString> UnlockedShips;
};
