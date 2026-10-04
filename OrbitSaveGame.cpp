// OrbitSaveGame.cpp
// ---------------
// Implements binary serialization of the Orbit game state.
// The save format is a simple binary blob written to
// <ProjectSavedDir>/OrbitGame.sav.  The format is
// intentionally minimal – it contains only the data that
// is required to restore the game to a consistent state
// after a restart.
//
// The implementation uses UE5's FArchive system so that
// the same code works on all supported platforms
// (Windows, Linux, macOS, consoles, etc.).

#include "OrbitSaveGame.h"
#include "OrbitTypes.h"
#include "OrbitPhysicsSimulation.h"
#include "OrbitRenderCore.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // Helper: Convert a string to a platform‑independent path.
    // ------------------------------------------------------------------
    static FString GetSaveFilePath()
    {
        // All saves go into the project's Saved directory.
        return FPaths::Combine(
            FPaths::ProjectSavedDir(),
            TEXT("OrbitGame.sav")
        );
    }

    // ------------------------------------------------------------------
    // Serialize the entire game state into a binary buffer.
    // ------------------------------------------------------------------
    void UOrbitSaveGame::SerializeGameState(FBufferArchive& Ar)
    {
        // 1. Header – a simple magic number + version.
        //    This allows us to detect corrupted or incompatible files.
        static constexpr uint32 MagicNumber = 0x4F524954; // "ORIT"
        static constexpr uint32 Version = 1;

        Ar << MagicNumber;
        Ar << Version;

        // 2. Engine configuration
        Ar << Config;

        // 3. Physics simulation state
        //    We assume OrbitPhysicsSimulation exposes a
        //    Serialize(FArchive&) method that writes all
        //    necessary data (rigid bodies, constraints, etc.).
        Physics.Serialize(Ar);

        // 4. Render core state
        //    For the purposes of a save game we only need to
        //    persist the camera position / orientation and any
        //    other user‑controlled view state.
        Renderer.Serialize(Ar);

        // 5. Frame counter (optional – useful for debugging)
        Ar << FrameNumber;
    }

    // ------------------------------------------------------------------
    // Deserialize the game state from a binary buffer.
    // ------------------------------------------------------------------
    bool UOrbitSaveGame::DeserializeGameState(FMemoryReader& Ar)
    {
        // 1. Header – verify magic number & version.
        uint32 MagicNumber = 0;
        uint32 Version = 0;
        Ar << MagicNumber;
        Ar << Version;

        if (MagicNumber != 0x4F524954 || Version != 1)
        {
            UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Unsupported or corrupted file."));
            return false;
        }

        // 2. Engine configuration
        Ar << Config;

        // 3. Physics simulation state
        if (!Physics.Deserialize(Ar))
        {
            UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to deserialize physics state."));
            return false;
        }

        // 4. Render core state
        if (!Renderer.Deserialize(Ar))
        {
            UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to deserialize render state."));
            return false;
        }

        // 5. Frame counter
        Ar << FrameNumber;

        return true;
    }

    // ------------------------------------------------------------------
    // Public API – write the current state to disk.
    // ------------------------------------------------------------------
    bool UOrbitSaveGame::SaveToDisk()
    {
        FBufferArchive Ar;
        SerializeGameState(Ar);

        const FString FilePath = GetSaveFilePath();

        // Write the buffer to disk.  FileHelper::SaveArrayToFile
        // handles platform‑specific file I/O and returns true on success.
        bool bSuccess = FFileHelper::SaveArrayToFile(
            Ar,
            *FilePath,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IPlatformFile::GetPlatformPhysical(),
            FILEWRITE_EvenIfReadOnly
        );

        if (!bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to write file %s"), *FilePath);
        }

        // Free the memory used by the archive.
        Ar.FlushCache();
        Ar.Empty();

        return bSuccess;
    }

    // ------------------------------------------------------------------
    // Public API – load the state from disk.
    // ------------------------------------------------------------------
    bool UOrbitSaveGame::LoadFromDisk()
    {
        const FString FilePath = GetSaveFilePath();

        // Load the file into a byte array.
        TArray<uint8> FileData;
        bool bSuccess = FFileHelper::LoadFileToArray(
            FileData,
            *FilePath,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IPlatformFile::GetPlatformPhysical()
        );

        if (!bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("OrbitSaveGame: Failed to read file %s"), *FilePath);
            return false;
        }

        // Deserialize from the buffer.
        FMemoryReader Ar(FileData, true);
        bool bDeserialized = DeserializeGameState(Ar);

        // Clean up the reader.
        Ar.FlushCache();
        Ar.Empty();

        return bDeserialized;
    }

    // ------------------------------------------------------------------
    // Helper: Reset the save game to a clean state.
    // ------------------------------------------------------------------
    void UOrbitSaveGame::Reset()
    {
        Config = EngineConfig{};
        Physics.Reset();
        Renderer.Reset();
        FrameNumber = 0;
    }
}
