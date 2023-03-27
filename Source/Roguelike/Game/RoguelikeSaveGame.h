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
	bool HaveTooltip;
	FString TooltipText;
	

	friend FArchive& operator<<(FArchive& Ar, FItemInfoRecord& InfoRecord)
	{
		Ar << InfoRecord.ItemsType;
		Ar << InfoRecord.DetailType;
		Ar << InfoRecord.ItemName;
		Ar << InfoRecord.ItemDesc;
		Ar << InfoRecord.HaveTooltip;
		Ar << InfoRecord.TooltipText;
		
		return Ar;
	}
};



UCLASS(Blueprintable)
class ROGUELIKE_API URoguelikeSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	URoguelikeSaveGame();
	
	bool SaveInfoRecord(FInfoRecord& Data);
	bool SaveItemInfos(TArray<UItemInfo*>& Data);
	void LoadInfoRecord(OUT FInfoRecord& Data);
	TArray<UItemInfo*> LoadItemInfos();

private:
	void CreateItemInfoRecord(UItemInfo* Info, FItemInfoRecord& Record);
	
	
	
};
