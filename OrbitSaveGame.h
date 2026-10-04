// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  UOrbitSaveGame
 *  ----------------
 *  A simple save game class that stores the player's fuel, score,
 *  current coordinates, and a list of unlocked ships.
 *
 *  All properties are exposed to the editor and Blueprints so they
 *  can be inspected or modified during gameplay.  The class also
 *  provides static helpers for loading and saving the game data.
 */
UCLASS()
class ORBIT_API UOrbitSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitSaveGame()
		: Fuel(0)
		, Score(0)
		, Coordinates(FVector::ZeroVector)
	{
	}

	/** Current fuel level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	int32 Fuel;

	/** Current score */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	int32 Score;

	/** Current world coordinates of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	FVector Coordinates;

	/** List of ship identifiers that have been unlocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
	TArray<FString> UnlockedShips;

	/**
	 *  Load a save game from the default slot.
	 *
	 *  @param SlotName  The name of the save slot (default: "OrbitSaveSlot").
	 *  @param UserIndex The user index (default: 0).
	 *  @return The loaded UOrbitSaveGame instance, or nullptr if loading failed.
	 */
	static UOrbitSaveGame* LoadGame(const FString& SlotName = TEXT("OrbitSaveSlot"),
	                                const int32 UserIndex = 0)
	{
		if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
		{
			return Cast<UOrbitSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		}
		return nullptr;
	}

	/**
	 *  Save a UOrbitSaveGame instance to the default slot.
	 *
	 *  @param SaveGame  The instance to save.
	 *  @param SlotName  The name of the save slot (default: "OrbitSaveSlot").
	 *  @param UserIndex The user index (default: 0).
	 *  @return true if the save succeeded, false otherwise.
	 */
	static bool SaveGame(UOrbitSaveGame* SaveGame,
	                     const FString& SlotName = TEXT("OrbitSaveSlot"),
	                     const int32 UserIndex = 0)
	{
		if (!SaveGame)
		{
			return false;
		}
		return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
	}
};
