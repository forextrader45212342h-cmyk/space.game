// OrbitSaveGame.cpp
// ---------------
// Implements a minimal UE5 save‑game system that serialises the
// current game state to a binary .sav file and can restore it later.
//
// The implementation follows UE5 conventions:
//   • A USaveGame subclass (UOrbitSaveGame) that holds the data.
//   • Custom binary serialisation via FArchive.
//   • File I/O through FFileHelper / IFileManager.
//   • The file is stored in the project’s Saved directory with a
//     ".sav" extension.
//
// The code is intentionally lightweight – it can be expanded to
// include any additional state you need to persist.

#include "OrbitSaveGame.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

UOrbitSaveGame::UOrbitSaveGame()
{
    // Default constructor – UE will initialise properties automatically.
}

void UOrbitSaveGame::Serialize(FArchive& Ar)
{
    // Let the base class handle its own data first.
    Super::Serialize(Ar);

    // Serialize the game‑specific data.
    Ar << PlayerPosition;
    Ar << PlayerRotation;
    Ar << Inventory;
    Ar << CurrentLevelName;
    Ar << bIsGamePaused;
}

bool UOrbitSaveGame::SaveToFile(const FString& SlotName) const
{
    // Convert the UOrbitSaveGame object into a binary buffer.
    FBufferArchive ToBinary;
    ToBinary << *this;

    // Build the full path: <Project>/Saved/<SlotName>.sav
    const FString FilePath = FPaths::ProjectSavedDir() / (SlotName + TEXT(".sav"));

    // Write the buffer to disk.
    return FFileHelper::SaveArrayToFile(ToBinary, *FilePath);
}

bool UOrbitSaveGame::LoadFromFile(const FString& SlotName)
{
    // Build the full path: <Project>/Saved/<SlotName>.sav
    const FString FilePath = FPaths::ProjectSavedDir() / (SlotName + TEXT(".sav"));

    // Load the file into a byte array.
    TArray<uint8> BinaryArray;
    if (!FFileHelper::LoadFileToArray(BinaryArray, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load save file: %s"), *FilePath);
        return false;
    }

    // Deserialize the byte array back into this object.
    FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
    FromBinary.Seek(0);
    FromBinary << *this;

    return true;
}
