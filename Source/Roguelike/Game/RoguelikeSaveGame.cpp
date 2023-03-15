// Fill out your copyright notice in the Description page of Project Settings.
#include "RoguelikeSaveGame.h"

#include "Serialization/BufferArchive.h"
#include "Serialization/ObjectWriter.h"

void CreateInfoDataRecord(FInfoRecord& Data)
{
	//FMemoryWriter MemoryWriter = FMemoryWriter(Data.Data, true);
	//RLArchive Arc = RLArchive(MemoryWriter);

}

URoguelikeSaveGame::URoguelikeSaveGame()
{
	SaveSlotName = "InfoDataSaveSlot";
	SaveIndex = 0;
}

bool URoguelikeSaveGame::SaveInfoRecord(FInfoRecord& Data)
{
	bool Result = false;
	const FString FilePath = FPaths::ProjectSavedDir() + INFO_RECORD_SAVE_FILE_NAME;

	FBufferArchive BinaryData;
	FMemoryWriter Writer = FMemoryWriter(BinaryData, true);
	RLArchive Arc = RLArchive(Writer);

	Writer << Data;
	
	Result = FFileHelper::SaveArrayToFile(BinaryData, *FilePath);

	BinaryData.FlushCache();
	BinaryData.Empty();

	return Result;
}

bool URoguelikeSaveGame::SaveItemInfos(TArray<UItemInfo*>& Data)
{
	const FString FilePath = FPaths::ProjectSavedDir() + ITEM_INFO_SAVE_FILE_NAME;
	
	InfoRecords.Empty();

	for (auto ItemInfo : Data)
	{
		FItemInfoRecord Rec = FItemInfoRecord();
		CreateItemInfoRecord(ItemInfo, Rec);
		InfoRecords.Add(Rec);
	}
	
	FBufferArchive BinaryData;
	BinaryData << InfoRecords;

	if (BinaryData.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Data!"));
		return false;
	}

	if (FFileHelper::SaveArrayToFile(BinaryData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info Save Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info Save Fail"));
	}

	BinaryData.FlushCache();
	BinaryData.Empty();

	return true;
}

void URoguelikeSaveGame::CreateItemInfoRecord(UItemInfo* Info, FItemInfoRecord& Record)
{
	FObjectWriter MemoryWriter = FObjectWriter(Record.ByteData);
	RLArchive Arc = RLArchive(MemoryWriter);

	Record.ItemsType = Info->ItemsType;
	Record.DetailType = Info->DetailType;
	Record.ItemName = Info->ItemName;
	Record.ItemDesc = Info->ItemDesc;
	
	Info->Serialize(Arc);
}

void URoguelikeSaveGame::LoadInfoRecord(FInfoRecord& Data)
{
	const FString FilePath = FPaths::ProjectSavedDir() + INFO_RECORD_SAVE_FILE_NAME;

	FBufferArchive BinaryData;

	FFileHelper::LoadFileToArray(BinaryData, *FilePath);

	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	RLArchive Arc = RLArchive(FromBinary);
	FromBinary.Seek(0);
	
	
	SaveLoadInfoRecord(FromBinary, Data);
	
	FromBinary.FlushCache();
	BinaryData.Empty();
	FromBinary.Close();

}

TArray<UItemInfo*> URoguelikeSaveGame::LoadItemInfos()
{
	const FString FilePath = FPaths::ProjectSavedDir() + ITEM_INFO_SAVE_FILE_NAME;

	FBufferArchive BinaryData;

	if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrong file"));
		return TArray<UItemInfo*>();
	}
	
	if (BinaryData.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("갯수 없음!"));
		return TArray<UItemInfo*>();; 
	}
	FMemoryReader FromBinary = FMemoryReader(BinaryData, true); 
	FromBinary.Seek(0);
	
	FromBinary << InfoRecords;
	
	TArray<UItemInfo*> Data;
	
	for (auto InfoRecord : InfoRecords)
	{
		Data.Add(UItemInfo::ConstructItemInfo(InfoRecord.ItemsType, InfoRecord.DetailType, InfoRecord.ItemName, InfoRecord.ItemDesc, nullptr));
	}

	BinaryData.Empty();
	FromBinary.Close();

	return Data;
}

void URoguelikeSaveGame::SaveLoadInfoRecord(FArchive& Ar, FInfoRecord& Data)
{
	Ar << Data;
}

