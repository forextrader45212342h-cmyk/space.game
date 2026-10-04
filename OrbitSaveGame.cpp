// OrbitSaveGame.cpp
// ---------------
// Serialises the current game state to a binary .sav file and restores it back.
// The implementation follows UE5 conventions – a USTRUCT that can be
// streamed with the << operator, and a UCLASS that exposes the
// Save/Load functions to Blueprint/Editor if desired.

#include "OrbitSaveGame.h"
#include "OrbitGameState.h"          // The struct that holds the serialisable state
#include "OrbitEngine.h"             // Access to the engine singleton
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // Helper: Convert a FOrbitGameState to a binary blob
    // ------------------------------------------------------------------
    static bool SerializeGameState(const FOrbitGameState& State, TArray<uint8>& OutData)
    {
        FBufferArchive Ar;
        Ar << const_cast<FOrbitGameState&>(State);   // << expects a non‑const reference
        OutData = Ar;
        return true;
    }

    // ------------------------------------------------------------------
    // Helper: Convert a binary blob back into a FOrbitGameState
    // ------------------------------------------------------------------
    static bool DeserializeGameState(const TArray<uint8>& InData, FOrbitGameState& OutState)
    {
        FMemoryReader Ar(InData, true);
        Ar << OutState;
        return true;
    }

    // ------------------------------------------------------------------
    // Public API – Save the current state to a .sav file
    // ------------------------------------------------------------------
    bool UOrbitSaveGame::SaveGame(const FString& SlotName)
    {
        // 1. Grab the current state from the engine
        const FOrbitGameState CurrentState = OrbitEngine::Get()->GetGameState();

        // 2. Serialise it into a byte array
        TArray<uint8> SerializedData;
        if (!SerializeGameState(CurrentState, SerializedData))
        {
            UE_LOG(LogOrbit, Error, TEXT("Failed to serialise game state for slot '%s'."), *SlotName);
            return false;
        }

        // 3. Build the full path – <Saved>/<SlotName>.sav
        const FString FilePath = FPaths::Combine(
            FPaths::ProjectSavedDir(),
            SlotName + TEXT(".sav")
        );

        // 4. Write the array to disk
        if (!FFileHelper::SaveArrayToFile(SerializedData, *FilePath))
        {
            UE_LOG(LogOrbit, Error, TEXT("Failed to write save file '%s'."), *FilePath);
            return false;
        }

        UE_LOG(LogOrbit, Log, TEXT("Game state successfully saved to '%s'."), *FilePath);
        return true;
    }

    // ------------------------------------------------------------------
    // Public API – Load a previously saved state from a .sav file
    // ------------------------------------------------------------------
    bool UOrbitSaveGame::LoadGame(const FString& SlotName)
    {
        // 1. Build the full path – <Saved>/<SlotName>.sav
        const FString FilePath = FPaths::Combine(
            FPaths::ProjectSavedDir(),
            SlotName + TEXT(".sav")
        );

        // 2. Read the file into a byte array
        TArray<uint8> FileData;
        if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
        {
            UE_LOG(LogOrbit, Error, TEXT("Failed to load save file '%s'."), *FilePath);
            return false;
        }

        // 3. Deserialise into a state struct
        FOrbitGameState LoadedState;
        if (!DeserializeGameState(FileData, LoadedState))
        {
            UE_LOG(LogOrbit, Error, TEXT("Failed to deserialise game state from '%s'."), *FilePath);
            return false;
        }

        // 4. Push the state back into the engine
        OrbitEngine::Get()->SetGameState(LoadedState);

        UE_LOG(LogOrbit, Log, TEXT("Game state successfully loaded from '%s'."), *FilePath);
        return true;
    }
}
