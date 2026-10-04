// OrbitSaveGame.cpp
// Implements binary serialization of the Orbit game state to a .sav file.

#include "OrbitSaveGame.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

bool UOrbitSaveGame::SaveToFile(const FString& FilePath)
{
    // Serialize the game state into a binary buffer.
    FBufferArchive ToBinary;
    ToBinary << GameState;

    // Write the binary buffer to disk.
    if (!FFileHelper::SaveArrayToFile(ToBinary, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to write save file '%s'."), *FilePath);
        ToBinary.FlushCache();
        ToBinary.Empty();
        return false;
    }

    // Clean up the archive.
    ToBinary.FlushCache();
    ToBinary.Empty();

    UE_LOG(LogTemp, Log, TEXT("OrbitSaveGame: Game state successfully saved to '%s'."), *FilePath);
    return true;
}

bool UOrbitSaveGame::LoadFromFile(const FString& FilePath)
{
    // Load the binary data from disk into an array.
    TArray<uint8> BinaryArray;
    if (!FFileHelper::LoadFileToArray(BinaryArray, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to load save file '%s'."), *FilePath);
        return false;
    }

    // Deserialize the binary data back into the game state.
    FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
    FromBinary.Seek(0);
    FromBinary << GameState;

    UE_LOG(LogTemp, Log, TEXT("OrbitSaveGame: Game state successfully loaded from '%s'."), *FilePath);
    return true;
}

FString UOrbitSaveGame::GetDefaultSavePath()
{
    // Default path: <Project>/Saved/OrbitSave.sav
    return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("OrbitSave.sav"));
}
