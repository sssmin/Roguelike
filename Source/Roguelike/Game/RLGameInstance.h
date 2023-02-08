// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Roguelike/Type/DFSInterface.h"
#include "Roguelike/Type/Manage.h"
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
	void NewGame();
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
	TSharedPtr<DFSAgrt> DFS;

	FVector2Int MapSize;
	int32 ClearCount; //�� Ŭ���� ����
	TArray<FCell> Board;
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell; //�� �̵��� ������ �ٲ�
	int32 StageLevel; //Ŭ�����ϸ� 1 ����
	int32 BossCell;
	int32 BossPrevCell; //�÷��̾ ���� ��ġ�ϸ� BossCell �߰�
	bool bIsDiscoverdBoss; //BossPrevCell�� �����ϸ� true. 
	int32 TotalCellNum; // �� �Ѱ���. �������� ������. Total - 1 == ClearCount�� ���� ���� ����
	
	FVector PlayerSpawnLoc;
	
	//������ ����, �÷��̾� ������ �����ؾ���.
	FHealthManage HealthManage;
	FCombatManage CombatManage;
	FItemManage ItemManage;
	uint8 Buff;
	uint8 EquipItem;

	int32 CalcNextCell(int32 Dir);
	UFUNCTION()
	void MoveNextStage(const FString& MapName);
public:
	void GetManager(OUT FHealthManage& OutHealthManage, OUT FCombatManage& OutCombatManage, OUT FItemManage& OutItemManage, OUT uint8& OutBuff)
	{ 
		OutHealthManage = HealthManage;
		OutCombatManage = CombatManage;
		OutItemManage = ItemManage;
		OutBuff = Buff;
		
	}
	void SetManager(FHealthManage& InHealthManage, FCombatManage& InCombatManage, FItemManage& InItemManage, uint8& InBuff)
	{
		HealthManage = InHealthManage;
		CombatManage = InCombatManage;
		ItemManage = InItemManage;
		Buff = InBuff;
	}
	int32 GetStageLevel() const { return StageLevel; }
	FCell GetCellInfo() const { return Board[PlayerCurrentCell]; }
	FVector GetPlayerSpawnLoc() const { return PlayerSpawnLoc; }
};
