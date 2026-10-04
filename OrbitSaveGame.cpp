// OrbitSaveGame.cpp
// Implements binary serialization for Orbit's save‑game data.

#include "OrbitSaveGame.h"
#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Engine/Engine.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

// -----------------------------------------------------------------------------
// Helper: Log a message to the UE5 output log.
// -----------------------------------------------------------------------------
static void Log(const FString& Message)
{
#if WITH_EDITOR
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Message);
    }
#endif
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}

// -----------------------------------------------------------------------------
// Save the current game state to a binary .sav file.
// -----------------------------------------------------------------------------
bool UOrbitSaveGame::SaveToFile(const FString& FilePath)
{
    // Serialize this UObject into a byte array.
    TArray<uint8> SaveData;
    FBufferArchive Writer(SaveData, true);

    // The << operator for UObject will call Serialize internally.
    Writer << this;

    // Write the byte array to disk.
    if (!FFileHelper::SaveArrayToFile(SaveData, *FilePath))
    {
        Log(FString::Printf(TEXT("OrbitSaveGame::SaveToFile FAILED: %s"), *FilePath));
        return false;
    }

    Log(FString::Printf(TEXT("OrbitSaveGame::SaveToFile SUCCESS: %s"), *FilePath));
    return true;
}

// -----------------------------------------------------------------------------
// Load a game state from a binary .sav file.
// -----------------------------------------------------------------------------
UOrbitSaveGame* UOrbitSaveGame::LoadFromFile(const FString& FilePath)
{
    // Read the file into a byte array.
    TArray<uint8> LoadData;
    if (!FFileHelper::LoadFileToArray(LoadData, *FilePath))
    {
        Log(FString::Printf(TEXT("OrbitSaveGame::LoadFromFile FAILED: %s"), *FilePath));
        return nullptr;
    }

    // Create a new instance of the save game object.
    UOrbitSaveGame* LoadedGame = NewObject<UOrbitSaveGame>();
    if (!LoadedGame)
    {
        Log(TEXT("OrbitSaveGame::LoadFromFile FAILED: Could not create UOrbitSaveGame instance."));
        return nullptr;
    }

    // Deserialize the byte array into the new object.
    FMemoryReader Reader(LoadData, true);
    Reader << LoadedGame;

    Log(FString::Printf(TEXT("OrbitSaveGame::LoadFromFile SUCCESS: %s"), *FilePath));
    return LoadedGame;
}

// -----------------------------------------------------------------------------
// Convenience wrapper: Save the current game state to the default slot.
// -----------------------------------------------------------------------------
bool UOrbitSaveGame::SaveToDefaultSlot()
{
    const FString SlotName = TEXT("OrbitDefaultSlot");
    const int32 UserIndex = 0;

    UOrbitSaveGame* SaveGameInstance = Cast<UOrbitSaveGame>(UGameplayStatics::CreateSaveGameObject(UOrbitSaveGame::StaticClass()));
    if (!SaveGameInstance)
    {
        Log(TEXT("OrbitSaveGame::SaveToDefaultSlot FAILED: Could not create save game object."));
        return false;
    }

    // Copy the current state into the new instance.
    *SaveGameInstance = *this;

    return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
}

// -----------------------------------------------------------------------------
// Convenience wrapper: Load the game state from the default slot.
// -----------------------------------------------------------------------------
UOrbitSaveGame* UOrbitSaveGame::LoadFromDefaultSlot()
{
    const FString SlotName = TEXT("OrbitDefaultSlot");
    const int32 UserIndex = 0;

    if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        Log(TEXT("OrbitSaveGame::LoadFromDefaultSlot FAILED: No save slot found."));
        return nullptr;
    }

    UOrbitSaveGame* LoadedGame = Cast<UOrbitSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!LoadedGame)
    {
        Log(TEXT("OrbitSaveGame::LoadFromDefaultSlot FAILED: Could not load save game."));
    }
    return LoadedGame;
}
