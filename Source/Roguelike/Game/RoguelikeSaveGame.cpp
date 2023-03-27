// Fill out your copyright notice in the Description page of Project Settings.
#include "RoguelikeSaveGame.h"

#include "Serialization/BufferArchive.h"
#include "Serialization/ObjectWriter.h"


URoguelikeSaveGame::URoguelikeSaveGame()
{
}

bool URoguelikeSaveGame::SaveInfoRecord(FInfoRecord& Data)
{
	bool Result = false;
	const FString FilePath = FPaths::ProjectSavedDir() + INFO_RECORD_SAVE_FILE_NAME;

	FBufferArchive ToBinary;
	RLArchive Arc = RLArchive(ToBinary);

	ToBinary << Data;
	
	if (ToBinary.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No exist"));
		return false;
	}
	
	Result = FFileHelper::SaveArrayToFile(ToBinary, *FilePath);
	if (Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Other Info Save Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Other Info Save Fail"));
	}

	ToBinary.FlushCache();
	ToBinary.Empty();
	ToBinary.Close();

	return Result;
}

bool URoguelikeSaveGame::SaveItemInfos(TArray<UItemInfo*>& Data)
{
	bool Result = false;
	const FString FilePath = FPaths::ProjectSavedDir() + ITEM_INFO_SAVE_FILE_NAME;
	TArray<FItemInfoRecord> InfoRecords;

	for (auto ItemInfo : Data)
	{
		FItemInfoRecord Rec = FItemInfoRecord();
		CreateItemInfoRecord(ItemInfo, Rec);
		InfoRecords.Add(Rec);
	}
	
	FBufferArchive ToBinary;
	ToBinary << InfoRecords; //직렬화

	if (ToBinary.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No exist"));
		return false;
	}
	
	Result = FFileHelper::SaveArrayToFile(ToBinary, *FilePath);
	if (Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info Save Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info Save Fail"));
	}

	ToBinary.FlushCache();
	ToBinary.Empty();
	ToBinary.Close();

	return Result;
}

void URoguelikeSaveGame::CreateItemInfoRecord(UItemInfo* Info, FItemInfoRecord& Record)
{
	Record.ItemsType = Info->ItemsType;
	Record.DetailType = Info->DetailType;
	Record.ItemName = Info->ItemName;
	Record.ItemDesc = Info->ItemDesc;
	Record.HaveTooltip = Info->HaveTooltip;
	Record.TooltipText = Info->TooltipText;
}

void URoguelikeSaveGame::LoadInfoRecord(OUT FInfoRecord& Data)
{
	const FString FilePath = FPaths::ProjectSavedDir() + INFO_RECORD_SAVE_FILE_NAME;

	FBufferArchive BinaryData;

	if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrong file"));
		return;
	}

	if (BinaryData.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No exist"));
		return; 
	}

	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	RLArchive Arc = RLArchive(FromBinary);
	FromBinary.Seek(0);
	
	FromBinary << Data;
	
	BinaryData.Empty();
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
		UE_LOG(LogTemp, Warning, TEXT("No exist"));
		return TArray<UItemInfo*>();; 
	}
	FMemoryReader FromBinary = FMemoryReader(BinaryData, true); 
	FromBinary.Seek(0);
	TArray<FItemInfoRecord> InfoRecords;
	FromBinary << InfoRecords;
	
	TArray<UItemInfo*> Data;
	
	for (auto InfoRecord : InfoRecords)
	{
		Data.Add(UItemInfo::ConstructItemInfo(InfoRecord.ItemsType, InfoRecord.DetailType, InfoRecord.ItemName, InfoRecord.ItemDesc, nullptr, InfoRecord.HaveTooltip, InfoRecord.TooltipText));
	}

	BinaryData.Empty();
	FromBinary.Close();

	return Data;
}
