// OrbitSaveGame.cpp
// ---------------
// Serialises the current engine state to a binary .sav file and restores it back.
// Uses UE5's USaveGame infrastructure and low‑level FArchive for binary
// serialization.  The file format is intentionally simple – a
// FMemoryWriter/FMemoryReader pair – so that it can be inspected with
// standard tools if needed.

#include "OrbitSaveGame.h"
#include "OrbitEngineMainLoop.h"
#include "OrbitPhysicsSimulation.h"
#include "OrbitRenderCore.h"

#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// Helper: Serialise/Deserialise a struct that contains the engine state
//////////////////////////////////////////////////////////////////////////

// The struct that will be written to disk.  It must be POD‑like and
// contain only serialisable members (USTRUCT with GENERATED_BODY() and
// the appropriate UPROPERTY flags).
// NOTE: In a real project you would probably serialise more data
// (e.g. world objects, player state, etc.).  For the purposes of this
// example we only serialise the engine config and the current frame
// number.
USTRUCT(BlueprintType)
struct FOrbitGameState
{
    GENERATED_BODY()

    // Engine configuration – serialised via the EngineConfig struct
    // defined in OrbitTypes.h.  We expose it as a UPROPERTY so that
    // the UE4 reflection system can serialise it automatically.
    UPROPERTY()
    EngineConfig Config;

    // Current frame number
    UPROPERTY()
    uint64 FrameNumber;

    // Physics simulation state – we serialise the entire simulation
    // object.  In practice you might want to serialise only the
    // necessary data (e.g. rigid bodies, constraints, etc.).
    UPROPERTY()
    OrbitPhysicsSimulation Physics;

    // Render core state – usually you don't serialise the renderer,
    // but we keep it here for completeness.
    UPROPERTY()
    OrbitRenderCore RenderCore;

    // Default constructor
    FOrbitGameState()
        : FrameNumber(0)
    {}

    // Custom serialisation operator – this is required because
    // OrbitPhysicsSimulation and OrbitRenderCore are not UObjects.
    friend FArchive& operator<<(FArchive& Ar, FOrbitGameState& State)
    {
        Ar << State.Config;
        Ar << State.FrameNumber;
        Ar << State.Physics;
        Ar << State.RenderCore;
        return Ar;
    }
};

//////////////////////////////////////////////////////////////////////////
// UOrbitSaveGame
//////////////////////////////////////////////////////////////////////////

// The USaveGame subclass that will be used by the engine to persist
// the state.  It simply holds an FOrbitGameState instance.
UOrbitSaveGame::UOrbitSaveGame()
{
    // Nothing to do – the default constructor will initialise the
    // FOrbitGameState member.
}

//////////////////////////////////////////////////////////////////////////
// Public API – Save / Load
//////////////////////////////////////////////////////////////////////////

/**
 * Serialises the current engine state to a binary .sav file.
 *
 * @param InEngineMainLoop  The engine main loop instance that holds the
 *                          current state.
 * @param FileName          The file name (without path).  The file will
 *                          be written to <Project>/Saved/SaveGames/.
 * @return true if the file was written successfully.
 */
bool UOrbitSaveGame::SaveGameToFile(const EngineMainLoop& InEngineMainLoop, const FString& FileName)
{
    // Build the full path
    const FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
    const FString FullPath = FPaths::Combine(SaveDir, FileName + TEXT(".sav"));

    // Ensure the directory exists
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*SaveDir))
    {
        PlatformFile.CreateDirectoryTree(*SaveDir);
    }

    // Populate the state struct
    FOrbitGameState GameState;
    GameState.Config = InEngineMainLoop.GetConfig();          // Assume a getter
    GameState.FrameNumber = InEngineMainLoop.GetFrameNumber(); // Assume a getter
    GameState.Physics = InEngineMainLoop.GetPhysics();        // Assume a getter
    GameState.RenderCore = InEngineMainLoop.GetRenderer();    // Assume a getter

    // Serialise to a memory buffer
    FBufferArchive Archive;
    Archive << GameState;

    // Optionally compress the archive – this keeps the file size small
    // and is a common UE5 pattern for save games.
    FArchiveSaveCompressedProxy CompressedArchive(Archive, COMPRESS_ZLIB);
    TArray<uint8> CompressedData;
    CompressedArchive << CompressedData;

    // Write the compressed data to disk
    bool bSuccess = FFileHelper::SaveArrayToFile(CompressedData, *FullPath);

    // Clean up
    Archive.FlushCache();
    Archive.Empty();

    return bSuccess;
}

/**
 * Loads a previously saved game from disk and restores the engine state.
 *
 * @param OutEngineMainLoop  The engine main loop instance that will be
 *                           updated with the loaded state.
 * @param FileName           The file name (without path).  The file is
 *                           read from <Project>/Saved/SaveGames/.
 * @return true if the file was read and deserialised successfully.
 */
bool UOrbitSaveGame::LoadGameFromFile(EngineMainLoop& OutEngineMainLoop, const FString& FileName)
{
    // Build the full path
    const FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
    const FString FullPath = FPaths::Combine(SaveDir, FileName + TEXT(".sav"));

    // Read the file into a byte array
    TArray<uint8> CompressedData;
    if (!FFileHelper::LoadFileToArray(CompressedData, *FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load save file: %s"), *FullPath);
        return false;
    }

    // Decompress the data
    FArchiveLoadCompressedProxy DecompressArchive(CompressedData, COMPRESS_ZLIB);
    TArray<uint8> DecompressedData;
    DecompressArchive << DecompressedData;

    // Read the decompressed data into a memory reader
    FMemoryReader Reader(DecompressedData, true);
    Reader.Seek(0);

    // Deserialise into the state struct
    FOrbitGameState GameState;
    Reader << GameState;

    // Apply the loaded state to the engine
    OutEngineMainLoop.SetConfig(GameState.Config);          // Assume a setter
    OutEngineMainLoop.SetFrameNumber(GameState.FrameNumber); // Assume a setter
    OutEngineMainLoop.SetPhysics(GameState.Physics);        // Assume a setter
    OutEngineMainLoop.SetRenderer(GameState.RenderCore);    // Assume a setter

    // Clean up
    Reader.FlushCache();
    Reader.Close();

    return true;
}

//////////////////////////////////////////////////////////////////////////
// Optional: Convenience wrappers for the game instance
//////////////////////////////////////////////////////////////////////////

/**
 * Convenience wrapper that can be called from Blueprint or C++.
 * It uses the global engine instance to get the main loop.
 */
UFUNCTION(BlueprintCallable, Category = "SaveGame")
bool UOrbitSaveGame::SaveCurrentGame(const FString& FileName)
{
    if (!GEngine)
    {
        UE_LOG(LogTemp, Error, TEXT("GEngine is null – cannot save game."));
        return false;
    }

    // Retrieve the engine main loop from the game instance
    EngineMainLoop* MainLoop = GEngine->GetMainLoop(); // Hypothetical accessor
    if (!MainLoop)
    {
        UE_LOG(LogTemp, Error, TEXT("MainLoop is null – cannot save game."));
        return false;
    }

    return SaveGameToFile(*MainLoop, FileName);
}

UFUNCTION(BlueprintCallable, Category = "SaveGame")
bool UOrbitSaveGame::LoadCurrentGame(const FString& FileName)
{
    if (!GEngine)
    {
        UE_LOG(LogTemp, Error, TEXT("GEngine is null – cannot load game."));
        return false;
    }

    EngineMainLoop* MainLoop = GEngine->GetMainLoop(); // Hypothetical accessor
    if (!MainLoop)
    {
        UE_LOG(LogTemp, Error, TEXT("MainLoop is null – cannot load game."));
        return false;
    }

    return LoadGameFromFile(*MainLoop, FileName);
}
