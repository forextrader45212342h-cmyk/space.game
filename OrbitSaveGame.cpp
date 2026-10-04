// OrbitSaveGame.cpp
// ---------------
// Implements binary serialization of the game's state to a .sav file.
// Uses UE5's FArchive system so the code stays fully UE‑compatible.

#include "OrbitSaveGame.h"
#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // Helper: serialise a GameState instance to a binary buffer.
    // ------------------------------------------------------------------
    static void SerializeGameState(FBufferArchive& Ar, const GameState& State)
    {
        // The GameState struct must expose an operator<< that knows how to
        // serialise its members.  The implementation is in OrbitSaveGame.h.
        Ar << State;
    }

    // ------------------------------------------------------------------
    // Helper: deserialise a GameState instance from a binary buffer.
    // ------------------------------------------------------------------
    static void DeserializeGameState(FMemoryReader& Ar, GameState& State)
    {
        Ar << State;
    }

    // ------------------------------------------------------------------
    // Public API: Save the current game state to a .sav file.
    // ------------------------------------------------------------------
    bool SaveGame(const FString& FilePath, const GameState& State)
    {
        // 1. Pack the state into a binary archive.
        FBufferArchive ToBinary;
        SerializeGameState(ToBinary, State);

        // 2. Write the archive to disk.
        bool bSuccess = FFileHelper::SaveArrayToFile(
            ToBinary,
            *FilePath,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IFileManager::Get(),
            FILEWRITE_EvenIfReadOnly
        );

        // 3. Clean up the archive.
        ToBinary.FlushCache();
        ToBinary.Empty();

        return bSuccess;
    }

    // ------------------------------------------------------------------
    // Public API: Load a game state from a .sav file.
    // ------------------------------------------------------------------
    bool LoadGame(const FString& FilePath, GameState& OutState)
    {
        // 1. Read the raw file into a byte array.
        TArray<uint8> BinaryArray;
        if (!FFileHelper::LoadFileToArray(BinaryArray, *FilePath))
        {
            return false; // file not found / could not be read
        }

        // 2. Create a memory reader over the array.
        FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
        FromBinary.Seek(0);

        // 3. Unpack the state.
        DeserializeGameState(FromBinary, OutState);

        // 4. Clean up.
        FromBinary.FlushCache();
        BinaryArray.Empty();

        return true;
    }
}
