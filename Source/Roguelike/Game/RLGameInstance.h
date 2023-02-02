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

private:
	FVector2Int MapSize;
	TSharedPtr<DFSAgrt> DFS;
	int32 ClearCount; //�� Ŭ���� ����
	TArray<FCell> Board;
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell; //�� �̵��� ������ �ٲ�
	int32 StageLevel; //Ŭ�����ϸ� 1 ����
	int32 BossCell;
	int32 BossPrevCell; //�÷��̾ ���� ��ġ�ϸ� BossCell �߰�
	bool bIsDiscoverdBoss; //BossPrevCell�� �����ϸ� true. 
	int32 TotalRoomNum; // �� �Ѱ���. �������� ������. Total - 1 == ClearCount�� ���� ���� ����

	FVector PlayerSpawnLoc;
	//��ġ�� ���� ���� ���̳Ŀ� ���� ���ӽ�����Ʈ�� ����� ������ �ٸ���
	//�� ���� �� �׷��͵��� ���� ����ؼ� �ٱ�?
	//�ƴϸ� �������������, �̹� Ÿ�Ը� �ְ�
	//���������� ���� ���ӽ�����Ʈ���� ���������̺��� �������ұ�?
	// 
	//������ ����, �÷��̾� ������ �����ؾ���.
	FManage Manager;

	int32 CalcNextCell(int32 Dir);
public:
	FManage GetManager() const { return Manager; }
	void SetManager(FManage InManager) { Manager = InManager; }
	int32 GetStageLevel() const { return StageLevel; }
	FCell GetCellInfo() const { return Board[PlayerCurrentCell]; }
	FVector GetPlayerSpawnLoc() const { return PlayerSpawnLoc;  }
};
