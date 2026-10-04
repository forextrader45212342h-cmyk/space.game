// OrbitSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitSaveGame.generated.h"

/**
 * Simple coordinate representation used for saving visited locations.
 */
USTRUCT(BlueprintType)
struct FOrbitCoordinate
{
    GENERATED_BODY()

    /** World location of the coordinate. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
    FVector Location = FVector::ZeroVector;

    /** Rotation at the coordinate (optional). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
    FRotator Rotation = FRotator::ZeroRotator;

    FOrbitCoordinate() = default;
};

/**
 * SaveGame class that stores fuel, score, visited coordinates and unlocked ships.
 */
UCLASS(BlueprintType)
class ORBIT_API UOrbitSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    /** Default constructor. */
    UOrbitSaveGame();

    /** Current fuel amount. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float Fuel = 0.f;

    /** Current score. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 Score = 0;

    /** List of coordinates the player has visited. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    TArray<FOrbitCoordinate> Coordinates;

    /** List of ship IDs that have been unlocked. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    TArray<int32> UnlockedShipIDs;

    /** Add a new coordinate to the list. */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void AddCoordinate(const FVector& InLocation, const FRotator& InRotation = FRotator::ZeroRotator);

    /** Unlock a ship by its ID. */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void UnlockShip(int32 ShipID);

    /** Check if a ship is already unlocked. */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    bool IsShipUnlocked(int32 ShipID) const;
};
