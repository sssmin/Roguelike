#pragma once
#include "StatManage.h"
#include "../Interface/DFSInterface.h"
#include "ItemManage.h"
#include "InfoData.generated.h"

USTRUCT(BlueprintType)
struct FInfoRecord
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<uint8, uint8> FixMaxNum;
	UPROPERTY()
	TArray<FCell> Board;
	
	FVector2Int MapSize;
	int32 ClearCount;
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell;
	int32 StageLevel; 
	int32 BossCell;
	int32 BossPrevCell;
	int32 TotalCellNum;
	bool bIsEarlyDiscoveredBoss;
	FHealthManager HealthManager;
	FCombatManager CombatManager;
	FItemManager ItemManager;
	uint8 Buff;
	FTransform PlayerTransform;
	int32 TempDashChargeNum;

	friend FArchive& operator<<(FArchive& Ar, FInfoRecord& InfoRecord)
	{
		Ar << InfoRecord.FixMaxNum;
		Ar << InfoRecord.Board;
		Ar << InfoRecord.MapSize;
		Ar << InfoRecord.ClearCount;
		Ar << InfoRecord.StartPos;
		Ar << InfoRecord.StartCell;
		Ar << InfoRecord.PlayerCurrentCell;
		Ar << InfoRecord.StageLevel;
		Ar << InfoRecord.BossCell;
		Ar << InfoRecord.BossPrevCell;
		Ar << InfoRecord.TotalCellNum;
		Ar << InfoRecord.bIsEarlyDiscoveredBoss;
		Ar << InfoRecord.HealthManager;
		Ar << InfoRecord.CombatManager;
		Ar << InfoRecord.ItemManager;
		Ar << InfoRecord.Buff;
		Ar << InfoRecord.PlayerTransform;
		Ar << InfoRecord.TempDashChargeNum;;

		return Ar;
	}
	
};

