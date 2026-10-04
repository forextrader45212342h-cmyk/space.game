// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 *  USaveGame subclass that stores the player's progress.
 *
 *  - Fuel: Current fuel level of the player's ship.
 *  - Score: Total score accumulated.
 *  - Coordinates: Current world position of the player.
 *  - UnlockedShips: List of ship identifiers that the player has unlocked.
 */
UCLASS(BlueprintType, Blueprintable, Category = "Orbit|Save")
class ORBIT_API UOrbitSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    /** Default constructor */
    UOrbitSaveGame();

    /** Current fuel level (0.0 – 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
    float Fuel = 0.0f;

    /** Total score earned by the player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
    int32 Score = 0;

    /** Current world position of the player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
    FVector Coordinates = FVector::ZeroVector;

    /** List of ship identifiers that have been unlocked */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Save")
    TArray<FString> UnlockedShips;
};
