// OrbitSaveGame.cpp
// ---------------
// Implements a UE5 USaveGame subclass that serializes the engine state
// (frame number, configuration, and any other game‑specific data) to a
// binary .sav file.  The class uses the standard UE5 save‑game system
// (UGameplayStatics::SaveGameToSlot / LoadGameFromSlot) but also
// demonstrates how to override Serialize() for custom binary layout
// if you need tighter control over the output format.

#include "OrbitSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// UOrbitSaveGame
//////////////////////////////////////////////////////////////////////////

// Called by the engine when the object is being serialized (e.g. when
// UGameplayStatics::SaveGameToSlot is invoked).  The default
// implementation of USaveGame already serializes all UPROPERTY members,
// but we override it here to show how you can add custom data or
// change the binary layout if required.
void UOrbitSaveGame::Serialize(FArchive& Ar)
{
    // Let the base class handle its own data first
    Super::Serialize(Ar);

    // Example: serialize the frame counter and the engine configuration.
    // If EngineConfig is a USTRUCT with UPROPERTY members, it will be
    // automatically serialized by the engine.  If you need a custom
    // binary format, you can write the fields manually as shown below.
    Ar << FrameNumber;
    Ar << Config;
}

//////////////////////////////////////////////////////////////////////////
// Public helper functions
//////////////////////////////////////////////////////////////////////////

// Saves the current UOrbitSaveGame instance to the specified slot.
// The slot name is used as the file name (e.g. "MySlot.sav") and
// UserIndex is the player index (usually 0 for single‑player games).
void UOrbitSaveGame::SaveGameToSlot(const FString& SlotName, int32 UserIndex)
{
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGameToSlot: Invalid SaveGame object"));
        return;
    }

    if (!UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot '%s'"), *SlotName);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Game successfully saved to slot '%s'"), *SlotName);
    }
}

// Loads a UOrbitSaveGame instance from the specified slot.  Returns
// nullptr if the slot does not exist or the load fails.
UOrbitSaveGame* UOrbitSaveGame::LoadGameFromSlot(const FString& SlotName, int32 UserIndex)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("No save game exists in slot '%s'"), *SlotName);
        return nullptr;
    }

    UOrbitSaveGame* LoadedGame = Cast<UOrbitSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!LoadedGame)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load game from slot '%s'"), *SlotName);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Game successfully loaded from slot '%s'"), *SlotName);
    }

    return LoadedGame;
}

//////////////////////////////////////////////////////////////////////////
// Optional: Manual binary export (if you want to write the file yourself)
// ---------------------------------------------------------------------
// The following helper functions show how you could write the save data
// to a raw .sav file using FBufferArchive / FMemoryReader.  This is
// rarely necessary because UGameplayStatics already handles the binary
// format, but it can be useful for debugging or when you need a
// platform‑specific file layout.

void UOrbitSaveGame::ExportToBinaryFile(const FString& FilePath)
{
    FBufferArchive ToBinary;
    ToBinary << *this; // Serializes the entire object

    if (FFileHelper::SaveArrayToFile(ToBinary, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Save data exported to '%s'"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to export save data to '%s'"), *FilePath);
    }

    ToBinary.FlushCache();
    ToBinary.Empty();
}

UOrbitSaveGame* UOrbitSaveGame::ImportFromBinaryFile(const FString& FilePath)
{
    TArray<uint8> BinaryData;
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load binary file '%s'"), *FilePath);
        return nullptr;
    }

    FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
    FromBinary.Seek(0);

    UOrbitSaveGame* ImportedGame = NewObject<UOrbitSaveGame>();
    ImportedGame->Serialize(FromBinary);

    FromBinary.FlushCache();
    BinaryData.Empty();

    UE_LOG(LogTemp, Log, TEXT("Save data imported from '%s'"), *FilePath);
    return ImportedGame;
}
