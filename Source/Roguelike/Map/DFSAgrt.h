// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/DFSInterface.h"


class ROGUELIKE_API DFSAgrt
{
public:
	DFSAgrt();
	~DFSAgrt();
	void StartAlgorithm(FVector2Int Size);
	void GetMaxWidgetHeight(OUT int32 MaxWidth, OUT int32 MaxHeight);
private:
	void MazeGenerator();
	void GenerateDungeon();
	void Init(); //������ �� ����(������ ����), ������, ������ ���� ����, ������ ������ ����
	TArray<int32> CheckSideCell(int32 Cell);

	FVector2Int Size;
	int32 StartPos;
	TArray<FCell> Board;
	int32 CellCount;
	int32 Parts; //���������� ���? ����
	int32 CreatedPart;
	int32 BossCell;
	int32 BossPrevCell;
	FVector StartPostion;
	int32 TotalCellNum;
	void ValueInit();
	
public:
	FVector GetStartPostion() const { return StartPostion; }
	TArray<FCell> GetBoard() const { return Board; }
	int32 GetStartCell() const { return StartPos; }
	int32 GetBossCell() const { return BossCell; }
	int32 GetBossPrevCell() const { return BossPrevCell; }
	int32 GetTotalCellNum() const { return TotalCellNum; }
};
