// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Roguelike/DFSInterface.h"
#include "Roguelike/Manage.h"
#include "RLGameInstance.generated.h"

class DFSAgrt;
DECLARE_DELEGATE(FOnMoveMap);

UCLASS()
class ROGUELIKE_API URLGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URLGameInstance();
	UFUNCTION(BlueprintCallable)
	void Initialize();
	void GenerateMap();
	void TestPrintMap();
	virtual void Init() override;
	void RequestInfo();
	void RequestMove(int32 Dir, const FVector& OtherSide);
	TArray<int32> GetConnectedDir();
	void ClearThisCell();
	FOnMoveMap OnMoveMap;
	void ClearStage();
private:
	FVector2Int MapSize;
	TSharedPtr<DFSAgrt> DFS;
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

	FVector PlayerSpawnLoc;
	
	//아이템 정보, 플레이어 정보도 저장해야함.
	FHealthManage HealthManage;
	FCombatManage CombatManage;
	uint8 State;

	int32 CalcNextCell(int32 Dir);
public:
	void GetManager(OUT FHealthManage& InHealthManage, OUT FCombatManage& InCombatManage, OUT uint8& InState)
	{ 
		InHealthManage = HealthManage;
		InCombatManage = CombatManage;
		InState = State;
	}
	void SetManager(FHealthManage& InHealthManage, FCombatManage& InCombatManage, uint8& InState) 
	{
		HealthManage = InHealthManage;
		CombatManage = InCombatManage;
		State = InState;
	}
	int32 GetStageLevel() const { return StageLevel; }
	FCell GetCellInfo() const { return Board[PlayerCurrentCell]; }
	FVector GetPlayerSpawnLoc() const { return PlayerSpawnLoc; }
};
