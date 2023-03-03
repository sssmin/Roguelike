// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Interface/DFSInterface.h"


class ROGUELIKE_API DFSAgrt
{
public:
	DFSAgrt();
	~DFSAgrt();
	void StartAlgorithm(FVector2Int Size);
	void GetMaxWidgetHeight(OUT int32 MaxWidth, OUT int32 MaxHeight);
	
private:
	void MazeGenerator();
	void MakeBonusCell();
	void Init(); //������ �� ����(������ ����), ������, ������ ���� ����, ������ ������ ����
	void ValueInit();
	TArray<int32> CheckSideCell(int32 Cell);

	FVector2Int Size;
	int32 StartCell;
	TArray<FCell> Board;
	int32 CellCount;
	int32 Parts; //���������� ���?
	int32 CreatedPart;
	int32 BossCell;
	int32 BossPrevCell;
	FVector StartPostion;
	int32 TotalCellNum;
	int32 BonusCellNum; 
	bool CreatedBossCell;
	
public:
	FVector GetStartPostion() const { return StartPostion; }
	TArray<FCell> GetBoard() const { return Board; }
	int32 GetStartCell() const { return StartCell; }
	int32 GetBossCell() const { return BossCell; }
	int32 GetBossPrevCell() const { return BossPrevCell; }
	int32 GetTotalCellNum() const { return TotalCellNum; }
	int32 GetBonusCellNum() const { return BonusCellNum;  }
	
};
