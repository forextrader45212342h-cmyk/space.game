// OrbitSaveGame.cpp
// ---------------
// Implements binary serialization for the Orbit save‑game system.
// The class UOrbitSaveGame is defined in OrbitSaveGame.h and derives from USaveGame.
// This file provides helper functions to write/read the game state to a .sav binary file
// as well as the standard UE5 slot‑based save/load helpers.

#include "OrbitSaveGame.h"

#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// UOrbitSaveGame
//////////////////////////////////////////////////////////////////////////

/**
 * Saves the current game state to a UE5 slot.
 *
 * @param SlotName  The name of the slot to write to.
 * @param UserIndex The user index (usually 0 for single‑player).
 */
void UOrbitSaveGame::SaveGameToSlot(const FString& SlotName, int32 UserIndex)
{
    // Create a new save‑game object of our custom type.
    UOrbitSaveGame* SaveGameInstance = Cast<UOrbitSaveGame>(UGameplayStatics::CreateSaveGameObject(UOrbitSaveGame::StaticClass()));
    if (!SaveGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UOrbitSaveGame instance."));
        return;
    }

    // ------------------------------------------------------------------
    // Populate the save‑game object with the current state.
    // ------------------------------------------------------------------
    // These assignments are just examples – replace them with your actual
    // game logic that pulls data from the world, player controller, etc.
    // ------------------------------------------------------------------
    if (GEngine && GEngine->GetWorld())
    {
        // Example: capture the player pawn's location & rotation.
        APawn* PlayerPawn = GEngine->GetWorld()->GetFirstPlayerController()->GetPawn();
        if (PlayerPawn)
        {
            SaveGameInstance->PlayerLocation = PlayerPawn->GetActorLocation();
            SaveGameInstance->PlayerRotation = PlayerPawn->GetActorRotation();
        }

        // Example: capture a simple health value.
        // (Replace with your own health component or variable.)
        // SaveGameInstance->PlayerHealth = ...;

        // Example: capture inventory items.
        // SaveGameInstance->Inventory = ...;
    }

    // Write the object to the specified slot.
    if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot '%s'."), *SlotName);
    }
}

/**
 * Loads a game state from a UE5 slot.
 *
 * @param SlotName  The name of the slot to read from.
 * @param UserIndex The user index (usually 0 for single‑player).
 * @return The loaded UOrbitSaveGame instance, or nullptr if the slot does not exist.
 */
UOrbitSaveGame* UOrbitSaveGame::LoadGameFromSlot(const FString& SlotName, int32 UserIndex)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Save slot '%s' does not exist."), *SlotName);
        return nullptr;
    }

    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
    return Cast<UOrbitSaveGame>(LoadedGame);
}

/**
 * Serializes the current game state to a binary .sav file.
 *
 * @param FilePath The full path to the file to write.
 * @return true if the file was written successfully.
 */
bool UOrbitSaveGame::SaveGameToBinary(const FString& FilePath)
{
    // Create a new save‑game object and populate it.
    UOrbitSaveGame* SaveGameInstance = Cast<UOrbitSaveGame>(UGameplayStatics::CreateSaveGameObject(UOrbitSaveGame::StaticClass()));
    if (!SaveGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UOrbitSaveGame instance for binary save."));
        return false;
    }

    // ------------------------------------------------------------------
    // Populate the save‑game object with the current state.
    // ------------------------------------------------------------------
    if (GEngine && GEngine->GetWorld())
    {
        APawn* PlayerPawn = GEngine->GetWorld()->GetFirstPlayerController()->GetPawn();
        if (PlayerPawn)
        {
            SaveGameInstance->PlayerLocation = PlayerPawn->GetActorLocation();
            SaveGameInstance->PlayerRotation = PlayerPawn->GetActorRotation();
        }
    }

    // ------------------------------------------------------------------
    // Serialize the object into a buffer archive.
    // ------------------------------------------------------------------
    FBufferArchive BinaryArchive;
    BinaryArchive << *SaveGameInstance;

    // ------------------------------------------------------------------
    // Write the buffer to disk.
    // ------------------------------------------------------------------
    bool bSuccess = FFileHelper::SaveArrayToFile(BinaryArchive, *FilePath);

    // Clean up the archive.
    BinaryArchive.FlushCache();
    BinaryArchive.Empty();

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write binary save to '%s'."), *FilePath);
    }

    return bSuccess;
}

/**
 * Loads a game state from a binary .sav file.
 *
 * @param FilePath The full path to the file to read.
 * @return The loaded UOrbitSaveGame instance, or nullptr on failure.
 */
UOrbitSaveGame* UOrbitSaveGame::LoadGameFromBinary(const FString& FilePath)
{
    TArray<uint8> BinaryData;
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read binary save file '%s'."), *FilePath);
        return nullptr;
    }

    // Create a memory reader from the binary data.
    FMemoryReader BinaryReader = FMemoryReader(BinaryData, true);
    BinaryReader.Seek(0);

    // Create a new save‑game object and deserialize into it.
    UOrbitSaveGame* LoadedGame = NewObject<UOrbitSaveGame>();
    BinaryReader << *LoadedGame;

    // Clean up the reader.
    BinaryReader.FlushCache();
    BinaryData.Empty();

    return LoadedGame;
}
