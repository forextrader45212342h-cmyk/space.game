// OrbitSaveGame.cpp
// ---------------
// Serialises the current game state to a binary .sav file and restores it back.
// Uses UE5's FArchive system so the data can be read/written in a platform‑independent
// binary format.  The implementation is intentionally minimal – you can extend
// `FOrbitSaveGameData` with any additional game state you need.

#include "OrbitSaveGame.h"

#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Misc/Paths.h"

namespace Orbit
{
    // ------------------------------------------------------------------
    // Helper struct that holds the data we want to persist.
    // ------------------------------------------------------------------
    FOrbitSaveGameData::FOrbitSaveGameData()
        : PlayerLocation(FVector::ZeroVector)
        , PlayerRotation(FRotator::ZeroRotator)
        , Health(100)
    {
    }

    // ------------------------------------------------------------------
    // Serialise the struct to a binary buffer.
    // ------------------------------------------------------------------
    void FOrbitSaveGameData::Serialize(FArchive& Ar)
    {
        Ar << PlayerLocation;
        Ar << PlayerRotation;
        Ar << Health;
        Ar << InventoryItemIDs;
    }

    // ------------------------------------------------------------------
    // Save the current state to a file.
    // ------------------------------------------------------------------
    bool FOrbitSaveGame::Save(const FString& FileName, const FOrbitSaveGameData& Data)
    {
        // 1. Write the data into a memory buffer.
        FBufferArchive BinaryArchive;
        Data.Serialize(BinaryArchive);

        // 2. Convert the buffer to a TArray<uint8> that can be written to disk.
        TArray<uint8> BinaryData;
        BinaryData.Append(BinaryArchive.GetData(), BinaryArchive.Num());

        // 3. Write the binary data to the specified file.
        const FString FullPath = FPaths::ProjectSavedDir() / FileName;
        bool bSuccess = FFileHelper::SaveArrayToFile(BinaryData, *FullPath);

        // 4. Clean up the archive.
        BinaryArchive.FlushCache();
        BinaryArchive.Empty();

        return bSuccess;
    }

    // ------------------------------------------------------------------
    // Load the state from a file.
    // ------------------------------------------------------------------
    bool FOrbitSaveGame::Load(const FString& FileName, FOrbitSaveGameData& OutData)
    {
        // 1. Read the binary file into a TArray<uint8>.
        const FString FullPath = FPaths::ProjectSavedDir() / FileName;
        TArray<uint8> BinaryData;
        if (!FFileHelper::LoadFileToArray(BinaryData, *FullPath))
        {
            return false;
        }

        // 2. Create a memory reader from the binary data.
        FMemoryReader BinaryReader(BinaryData, true);
        BinaryReader.Seek(0);

        // 3. Deserialize into the output struct.
        OutData.Serialize(BinaryReader);

        // 4. Clean up the reader.
        BinaryReader.FlushCache();
        BinaryReader.Close();

        return true;
    }
}
