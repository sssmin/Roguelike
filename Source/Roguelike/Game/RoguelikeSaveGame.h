// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Roguelike/Type/InfoData.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "RoguelikeSaveGame.generated.h"

#define INFO_RECORD_SAVE_FILE_NAME "/SaveGames/InfoRecordSaveData.sav"
#define ITEM_INFO_SAVE_FILE_NAME "/SaveGames/ItemInfoSaveData.sav"



class RLArchive : public FObjectAndNameAsStringProxyArchive
{
public:
	RLArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true;
		ArNoDelta = true;
	}
};

USTRUCT()
struct FItemInfoRecord
{
	GENERATED_BODY()

	EItemType ItemsType;

	FItemType DetailType;

	FString ItemName;

	FString ItemDesc;
	
	TArray<uint8> ByteData;

	friend FArchive& operator<<(FArchive& Ar, FItemInfoRecord& InfoRecord)
	{
		Ar << InfoRecord.ItemsType;
		Ar << InfoRecord.DetailType;
		Ar << InfoRecord.ItemName;
		Ar << InfoRecord.ItemDesc;
		Ar << InfoRecord.ByteData;
		
		return Ar;
	}
};



UCLASS(Blueprintable)
class ROGUELIKE_API URoguelikeSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	URoguelikeSaveGame();
	UPROPERTY(BlueprintReadWrite)
	FString SaveSlotName;
	UPROPERTY(BlueprintReadWrite)
	int32 SaveIndex;
	UPROPERTY(SaveGame)
	FInfoRecord InfoData;
	UPROPERTY(BlueprintReadWrite)
	FTransform PlayerTransform;
	UPROPERTY(SaveGame)
	TArray<UItemInfo*> ItemInfos;
	UFUNCTION(BlueprintCallable)
	bool SaveInfoRecord(FInfoRecord& Data);
	UFUNCTION(BlueprintCallable)
	bool SaveItemInfos(TArray<UItemInfo*>& Data);
	void CreateItemInfoRecord(UItemInfo* Info, FItemInfoRecord& Record);
	UFUNCTION(BlueprintCallable)
	void LoadInfoRecord(FInfoRecord& Data);
	TArray<UItemInfo*> LoadItemInfos();
	UItemInfo* LoadItemInfoRecord(FItemInfoRecord& Record);
	UPROPERTY(SaveGame)
	TArray<FItemInfoRecord> InfoRecords;

	
	void SaveLoadInfoRecord(FArchive& Ar, FInfoRecord& Data);
	void SaveLoadItemInfos(FArchive& Ar, TArray<UItemInfo*>& Data);
	
	
};
