// OrbitSaveGame.cpp
// ---------------
// UE5 C++ implementation for serializing a USaveGame derived class to a binary .sav file.

#include "OrbitSaveGame.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

//////////////////////////////////////////////////////////////////////////
// UOrbitSaveGame
//////////////////////////////////////////////////////////////////////////

/**
 * Override the base USaveGame serialization routine.
 * All UPROPERTY members marked with `SaveGame` will be automatically
 * serialized by the engine, but we provide a manual implementation
 * here to demonstrate binary serialization to a .sav file.
 */
void UOrbitSaveGame::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);

    // Example properties that might exist in the save game.
    // Replace or extend these with your actual game state.
    Ar << OrbitalPositions;   // TArray<FVector>
    Ar << OrbitalTimes;       // TArray<float>
    Ar << PlayerScore;        // int32
    Ar << bIsGamePaused;      // bool
}

/**
 * Serializes the current UOrbitSaveGame instance to a binary buffer
 * and writes it to disk as a .sav file.
 *
 * @param SaveGame   The save game object to serialize.
 * @param FileName   The desired file name (without path). The file will be
 *                   written to the game's Saved directory.
 * @return true if the file was written successfully, false otherwise.
 */
bool UOrbitSaveGame::SaveGameToFile(UOrbitSaveGame* SaveGame, const FString& FileName)
{
    if (!SaveGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveGameToFile: SaveGame is null."));
        return false;
    }

    // Serialize the object into a binary buffer.
    FBufferArchive Ar;
    SaveGame->Serialize(Ar);

    // Build the full path: <GameDir>/Saved/SaveGames/<FileName>.sav
    FString FullPath = FPaths::Combine(
        FPaths::ProjectSavedDir(),
        TEXT("SaveGames"),
        FileName + TEXT(".sav")
    );

    // Ensure the directory exists.
    FPaths::CreateDirectoryTree(FPaths::GetPath(FullPath));

    // Write the buffer to disk.
    bool bSuccess = FFileHelper::SaveArrayToFile(Ar, *FullPath);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGameToFile: Failed to write %s"), *FullPath);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("SaveGameToFile: Successfully wrote %s"), *FullPath);
    }

    // Clean up the archive.
    Ar.FlushCache();
    Ar.Empty();

    return bSuccess;
}

/**
 * Loads a UOrbitSaveGame instance from a binary .sav file.
 *
 * @param FileName   The file name (without path) to load from.
 * @return A new UOrbitSaveGame instance populated with the loaded data,
 *         or nullptr if loading failed.
 */
UOrbitSaveGame* UOrbitSaveGame::LoadGameFromFile(const FString& FileName)
{
    // Build the full path: <GameDir>/Saved/SaveGames/<FileName>.sav
    FString FullPath = FPaths::Combine(
        FPaths::ProjectSavedDir(),
        TEXT("SaveGames"),
        FileName + TEXT(".sav")
    );

    if (!FPaths::FileExists(FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadGameFromFile: File %s does not exist."), *FullPath);
        return nullptr;
    }

    // Read the file into a byte array.
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("LoadGameFromFile: Failed to read %s"), *FullPath);
        return nullptr;
    }

    // Create a new instance of the save game class.
    UOrbitSaveGame* LoadedGame = NewObject<UOrbitSaveGame>();
    if (!LoadedGame)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadGameFromFile: Failed to create UOrbitSaveGame instance."));
        return nullptr;
    }

    // Deserialize the data into the new instance.
    FMemoryReader Ar(FileData, true);
    LoadedGame->Serialize(Ar);

    UE_LOG(LogTemp, Log, TEXT("LoadGameFromFile: Successfully loaded %s"), *FullPath);
    return LoadedGame;
}
