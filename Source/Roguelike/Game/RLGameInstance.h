// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Roguelike/Type/DFSInterface.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "RLGameInstance.generated.h"

class DFSAgrt;
class ARLGameStateBase;

DECLARE_DELEGATE(FOnMoveMap);

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
	TArray<int32> GetConnectedDir();
	void ClearThisCell();
	void AteHealThisCell();
	void ClearStage();


	FOnMoveMap OnMoveMap;
	void TestPrintMap();
private:
	TSharedPtr<DFSAgrt> DFS;
	UPROPERTY()
	ARLGameStateBase* RLGameState;

	FVector2Int MapSize;
	int32 ClearCount; //방 클리어 갯수
	TArray<FCell> Board;
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell; //맵 이동할 때마다 바뀜
	int32 StageLevel; //클리어하면 1 증가
	int32 BossCell;
	int32 BossPrevCell; //플레이어가 여기 위치하면 BossCell 발견
	bool bIsDiscoverdBoss; //BossPrevCell에 도착하면 true. 
	int32 TotalCellNum; // 방 총개수. 시작지점 미포함. Total - 1 == ClearCount면 보스 입장 가능
	
	//아이템 정보, 플레이어 정보도 저장해야함.
	FHealthManager HealthManager;
	FCombatManager CombatManager;
	FItemManager ItemManager;
	uint8 Buff;
	uint8 EquipItem;

	int32 CalcNextCell(int32 Dir);
	UFUNCTION()
	void MoveNextStage(const FString& MapName);
public:
	void GetManager(OUT FHealthManager& OutHealthManager, OUT FCombatManager& OutCombatManager, OUT uint8& OutBuff)
	{ 
		OutHealthManager = HealthManager;
		OutCombatManager = CombatManager;
		OutBuff = Buff;
		
	}
	void SetManager(FHealthManager& InHealthManager, FCombatManager& InCombatManager, uint8& InBuff)
	{
		HealthManager = InHealthManager;
		CombatManager = InCombatManager;
		Buff = InBuff;
	}
	int32 GetStageLevel() const { return StageLevel; }
	FCell GetCellInfo() const { return Board[PlayerCurrentCell]; }
};
