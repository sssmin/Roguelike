// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Roguelike/Type/DFSInterface.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Game/RLListenerManager.h"
#include "RLGameInstance.generated.h"

class DFSAgrt;
class ARLGameStateBase;
class ARLGameModeBase;

DECLARE_DELEGATE(FOnInitOnceItem)


UCLASS()
class ROGUELIKE_API URLGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URLGameInstance();
	virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	void NewGame();
	void Initialize();
	void GenerateMap();
	void RequestInfo();
	void RequestMove(int32 Dir);
	void RequestMoveNextStage();
	void RequestMovePrevBossCell();
	TArray<int32> GetConnectedDir();
	void ClearThisCell();
	void AteHealThisCell();
	void ReadyToRecall();
	void ShowSelectItemWidget();

	void TestPrintMap();

	URLListenerManager* GetListenerManager();
	
	FOnInitOnceItem InitOnceItemDelegate;
private:
	TSharedPtr<DFSAgrt> DFS;
	UPROPERTY()
	ARLGameStateBase* RLGameState;
	UPROPERTY()
	ARLGameModeBase* RLGameMode;

	FVector2Int MapSize;
	int32 ClearCount; //방 클리어 갯수
	TArray<FCell> Board;
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell; //맵 이동할 때마다 바뀜
	int32 StageLevel; //클리어하면 1 증가
	int32 BossCell;
	int32 BossPrevCell; //플레이어가 여기 위치하면 BossCell 발견
	int32 TotalCellNum; // 방 총개수. 시작지점 미포함. Total - 1 == ClearCount면 보스 입장 가능
	bool bIsEarlyDiscoveredBoss;
	
	FHealthManager HealthManager;
	FCombatManager CombatManager;
	FItemManager ItemManager;
	uint8 Buff;
	TMap<uint8, uint8> FixMaxNum;

	int32 CalcNextCell(int32 Dir);
	void CheckEarlyDiscoveredBossCell();
	void Recall();
	void MoveProcess(int32 TargetCell, int32 Dir);
	UPROPERTY()
	URLListenerManager* ListenerManager;
public:
	void GetManager(OUT FHealthManager& OutHealthManager, OUT FCombatManager& OutCombatManager, OUT uint8& OutBuff)
	{ 
		OutHealthManager = HealthManager;
		OutCombatManager = CombatManager;
		OutBuff = Buff;
		
	}
	void GetManager(OUT FItemManager& OutItemManager, OUT TMap<uint8, uint8>& OutFixMaxNum)
	{
		OutItemManager = ItemManager;
		OutFixMaxNum = FixMaxNum;

	}
	void SetManager(const FHealthManager& InHealthManager, const FCombatManager& InCombatManager, const uint8& InBuff)
	{
		HealthManager = InHealthManager;
		CombatManager = InCombatManager;
		Buff = InBuff;
	}
	void SetManager(FItemManager& InItemManager, TMap<uint8, uint8>& InFixMaxNum)
	{
		ItemManager = InItemManager;
		FixMaxNum = InFixMaxNum;
	}
	void SetSelectedBonusItem(bool Boolean)
	{
		if (Board.IsValidIndex(PlayerCurrentCell))
		{
			Board[PlayerCurrentCell].SelectedBonusItem = Boolean;
		}
	}
	FCell GetCellInfo() const 
	{ 
		if (Board.IsValidIndex(PlayerCurrentCell))
		{
			return Board[PlayerCurrentCell];
		}
		return FCell();
	}

	int32 GetStageLevel() const { return StageLevel; }
};
